#include <stdint.h>
#include "output.h"
#include "ringbuf.h"
#include "reg.h"

static RING_BUF usart_buf;

static int read_char(char *ch)
{
    if (USART1->SR & USART_SR_RXNE) {
        *ch = USART1->DR;
        return 0;
    } else {
        return -1;
    }
}

int usart_read(char *str, int len)
{
    int i, min_len;

    if (len > ringbuf_len(&usart_buf)) {
        min_len = ringbuf_len(&usart_buf);
    } else {
        min_len = len;
    }

    for (i = 0; i < min_len; i++) {
        ringbuf_read(&usart_buf, &str[i]);
    }
    return min_len;
}

void print_char(char ch)
{
    while (!(USART1->SR & USART_SR_TXE));
    USART1->DR = (ch & 0xFF);
}

void print_reg(void)
{
    register uint32_t reg_value;
    __asm__ volatile("mov %0, lr\n" : "=r"(reg_value));
    printfmt("lr=%x\r\n", reg_value);
    __asm__ volatile("mrs r0,control\nmov %0, r0\n" : "=r"(reg_value));
    printfmt("control=%x\r\n", reg_value);
}

void uart_init(void)
{
    /* Init serial buffer */
    ringbuf_init(&usart_buf);

    /* Init GPIO(GPIOA:AHB1) and USART(USART1:APB2ENR) clock */
#if 0
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
#else
    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
#endif

    /* GPIO Config, Rx->PA10, Tx->PA9 */
    GPIO_InitTypeDef gpio_tx_init = {
        .Pin = GPIO_PIN_9,
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FAST,
        .Alternate = GPIO_AF7_USART1,
    };
    GPIO_InitTypeDef gpio_rx_init = {
        .Pin = GPIO_PIN_10,
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FAST,
        .Alternate = GPIO_AF7_USART1,
    };
    HAL_GPIO_Init(GPIOA, &gpio_tx_init);
    HAL_GPIO_Init(GPIOA, &gpio_rx_init);

    /* USART1 Config: 8N1 no flow control */
    UART_HandleTypeDef uart_init = {
        .Instance = USART1,
        .Init = {
            .BaudRate = 115200,
            .WordLength = UART_WORDLENGTH_8B,
            .StopBits = UART_STOPBITS_1,
            .Parity = UART_PARITY_NONE,
            .HwFlowCtl = UART_HWCONTROL_NONE,
            .Mode = UART_MODE_TX_RX | UART_IT_RXNE,
            .OverSampling = UART_OVERSAMPLING_16,
        },
    };
    HAL_UART_Init(&uart_init);

    /* Enable interrupt from USART1 (NVIC level) */
    NVIC_EnableIRQ(USART1_IRQn);
}

void USART1_IRQHandler()
{
    char ch;
    if (read_char(&ch) != -1) {
        ringbuf_write(&usart_buf, ch);
    }
}

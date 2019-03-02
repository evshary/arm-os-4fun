#include <stdint.h>
#include "output.h"
#include "ringbuf.h"
#include "reg.h"

static RING_BUF usart_buf;

static int read_char(char *ch)
{
    if (USART2->SR & USART_SR_RXNE) {
        *ch = USART2->DR;
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
    while (!(USART2->SR & USART_SR_TXE));
    USART2->DR = (ch & 0xFF);
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

    /* Init GPIO and USART clock */
#if 0
    // RCC->APB2ENR |= (uint32_t)(0x00000001 | 0x00000004);
    // RCC->APB1ENR |= (uint32_t)(0x00020000);
#else
    __HAL_RCC_AFIO_CLK_ENABLE();   // RCC->APB2ENR: 0x00000001
    __HAL_RCC_GPIOA_CLK_ENABLE();  // RCC->APB2ENR: 0x00000004
    __HAL_RCC_USART2_CLK_ENABLE(); // RCC->APB1ENR: 0x00020000
#endif

    /* GPIO Config, Rx->PA3, Tx->PA2 */
#if 0
    GPIOA->CRL = 0x00004B00;
    GPIOA->CRH = 0x44444444;
    GPIOA->ODR = 0x00000000;
    GPIOA->BSRR = 0x00000000;
    GPIOA->BRR = 0x00000000;
#else
    GPIO_InitTypeDef gpio_tx_init = {
        .Pin = GPIO_PIN_2,
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_PULLUP,
        .Speed = GPIO_SPEED_FREQ_HIGH,
    };
    GPIO_InitTypeDef gpio_rx_init = {
        .Pin = GPIO_PIN_3,
        .Mode = GPIO_MODE_INPUT,
        .Pull = GPIO_PULLUP,
        .Speed = GPIO_SPEED_FREQ_HIGH,
    };
    HAL_GPIO_Init(GPIOA, &gpio_tx_init);
    HAL_GPIO_Init(GPIOA, &gpio_rx_init);
#endif

    /* USART2 Config: 8N1 no flow control */
#if 0
    USART2->CR1 = 0x0000000C;
    USART2->CR2 = 0x00000000;
    USART2->CR3 = 0x00000000;
    USART2->CR1 |= USART_CR1_UE; /* 0x2000 */
    USART2->CR1 |= USART_CR1_RXNEIE; /* 0x0020 */
#else
    UART_HandleTypeDef uart_init = {
        .Instance = USART2,
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
#endif

    /* Enable interrupt from USART2 (NVIC level) */
    NVIC_EnableIRQ(USART2_IRQn);
}

void USART2_IRQHandler()
{
    char ch;
    if (read_char(&ch) != -1) {
        ringbuf_write(&usart_buf, ch);
    }
}

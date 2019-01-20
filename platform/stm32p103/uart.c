#include <stdint.h>
#include "output.h"
#include "ringbuf.h"
#include "reg.h"

static RING_BUF usart2_buf;

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

    if (len > ringbuf_len(&usart2_buf)) {
        min_len = ringbuf_len(&usart2_buf);
    } else {
        min_len = len;
    }

    for (i = 0; i < min_len; i++) {
        ringbuf_read(&usart2_buf, &str[i]);
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
    ringbuf_init(&usart2_buf);

    /* Init USART */
    RCC->APB2ENR |= (uint32_t)(0x00000001 | 0x00000004);
    RCC->APB1ENR |= (uint32_t)(0x00020000);

    /* USART2 Configuration, Rx->PA3, Tx->PA2 */
    GPIOA->CRL = 0x00004B00;
    GPIOA->CRH = 0x44444444;
    GPIOA->ODR = 0x00000000;
    GPIOA->BSRR = 0x00000000;
    GPIOA->BRR = 0x00000000;

    USART2->CR1 = 0x0000000C;
    USART2->CR2 = 0x00000000;
    USART2->CR3 = 0x00000000;
    USART2->CR1 |= USART_CR1_UE; /* 0x2000 */
    USART2->CR1 |= USART_CR1_RXNEIE; /* 0x0020 */

    /* Enable interrupt from USART2 (NVIC level) */
    NVIC_EnableIRQ(USART2_IRQn);
}

void USART2_IRQHandler()
{
    char ch;
    if (read_char(&ch) != -1) {
        ringbuf_write(&usart2_buf, ch);
    }
}

#include <stdint.h>
#include "output.h"
#include "reg.h"

#define USART_FLAG_TXE  ((uint16_t) 0x0080)

void print_char(char ch)
{
    while (!(*(USART2_SR) & USART_FLAG_TXE));
    *(USART2_DR) = (ch & 0xFF);
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
    *(RCC_APB2ENR) |= (uint32_t)(0x00000001 | 0x00000004);
    *(RCC_APB1ENR) |= (uint32_t)(0x00020000);

    *(GPIOA_CRL) = 0x00004B00;
    *(GPIOA_CRH) = 0x44444444;
    *(GPIOA_ODR) = 0x00000000;
    *(GPIOA_BSRR) = 0x00000000;
    *(GPIOA_BRR) = 0x00000000;

    *(USART2_CR1) = 0x0000000C;
    *(USART2_CR2) = 0x00000000;
    *(USART2_CR3) = 0x00000000;
    *(USART2_CR1) |= 0x2000;
}

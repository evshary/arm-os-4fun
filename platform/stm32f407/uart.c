#include <stdint.h>
#include "output.h"
#include "reg.h"

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

static void uart_port_setup(void)
{
    /* Enable peripherals and GPIO Clocks */
    /* Enable GPIO TX/RX clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* Enable USART/UART clock */
    __HAL_RCC_USART1_CLK_ENABLE();
}

static void uart_port_init(void)
{
    /* GPIO initialized with USART/UART Tx/Rx configuration */
    GPIO_InitTypeDef gpio_tx_init_info = {
        .Pin = GPIO_PIN_9,
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FAST,
        .Alternate = GPIO_AF7_USART1,
    };
    GPIO_InitTypeDef gpio_rx_init_info = {
        .Pin = GPIO_PIN_10,
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FAST,
        .Alternate = GPIO_AF7_USART1,
    };
    HAL_GPIO_Init(GPIOA, &gpio_tx_init_info);
    HAL_GPIO_Init(GPIOA, &gpio_rx_init_info);

    /* UART init */
    UART_HandleTypeDef uart_init_info = {
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
    HAL_UART_Init(&uart_init_info);
}

void uart_init(void)
{
    uart_port_setup();
    uart_port_init();
}


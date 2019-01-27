#include <stdint.h>
#include "reg.h"
#include "usart.h"
#include "output.h"

extern void main(void);

extern uint32_t _flash_start_data;
extern uint32_t _start_data;
extern uint32_t _end_data;
extern uint32_t _start_bss;
extern uint32_t _end_bss;
extern uint32_t _end_stack;

void rcc_clock_init(void)
{
    /* Configure the System clock frequency, HCLK, PCLK2 and PCLK1 prescalers */
    /* Configure the Flash Latency cycles and enable prefetch buffer */
    volatile uint32_t StartUpCounter = 0, HSEStatus = 0;

    /* Enable HSE(External high-speed clock enable) */
    RCC->CR |= RCC_CR_HSEON;


    /* Wait till HSE is ready and if Time out is reached exit */
    do {
        HSEStatus = RCC->CR & RCC_CR_HSERDY;
        StartUpCounter++;
    } while ((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));


    if ((RCC->CR & RCC_CR_HSERDY) != 0) {
        HSEStatus = (uint32_t) 0x01;
    } else {
        HSEStatus = (uint32_t) 0x00;
    }

    if (HSEStatus == (uint32_t) 0x01) {
        /* Enable Prefetch Buffer */
        FLASH->ACR |= FLASH_ACR_PRFTEN;
        /* Flash clear wait state */
        FLASH->ACR &= ~FLASH_ACR_LATENCY;
        /* Flash 0 latency*/
        FLASH->ACR |= FLASH_ACR_LATENCY_0WS;

        /* AHB prescaler */
        /* HCLK = SYSCLK */
        RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
        /* APB high-speed prescalar */
        /* PCLK2 = HCLK */
        RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;
        /* APB low-speed prescalar */
        /* PCLK1 = HCLK */
        RCC->CFGR |= RCC_CFGR_PPRE1_DIV1;

        /* Select HSE as system clock source */
        RCC->CFGR &= ~RCC_CFGR_SW;
        RCC->CFGR |= RCC_CFGR_SW_HSE;

        /* Wait till HSE is used as system clock source */
        while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSE) {
            /*busy waiting*/
        }
    } else {
        /* If HSE fails to start-up, the application will have wrong clock
        configuration. User can add here some code to deal with this error */
    }
}

void reset_handler(void)
{
    uint32_t *flash_data_begin = &_flash_start_data;
    uint32_t *data_begin = &_start_data;
    uint32_t *data_end = &_end_data;
    while (data_begin < data_end) *data_begin++ = *flash_data_begin++;

    uint32_t *bss_begin = &_start_bss;
    uint32_t *bss_end = &_end_bss;
    while (bss_begin < bss_end) *bss_begin++ = 0;

    rcc_clock_init();
    main();
}

void nmi_handler(void)
{
    print_str("nmi fault\r\n");
    while (1);
}

void hardfault_handler(void)
{
    print_str("hard fault\r\n");
    while (1);
}

void memfault_handler(void)
{
    print_str("memory fault\r\n");
    while (1);
}

void busfault_handler(void)
{
    print_str("bus fault\r\n");
    while (1);
}

void usagefault_handler(void)
{
    print_str("usage fault\r\n");
    while (1);
}

void svc_handler(void);
void pendsv_handler(void);
void systick_handler(void);
void USART1_IRQHandler();

__attribute((section(".init_isr")))
uint32_t *isr_vectors[] = {
    (uint32_t *) &_end_stack,        /* stack pointer */
    (uint32_t *) reset_handler,      /* code entry point */
    (uint32_t *) nmi_handler,        /* NMI handler */
    (uint32_t *) hardfault_handler,  /* hard fault handler */
    (uint32_t *) memfault_handler,   /* memory fault handler */
    (uint32_t *) busfault_handler,   /* bus fault handler */
    (uint32_t *) usagefault_handler, /* usage fault handler */
    0,
    0,
    0,
    0,
    (uint32_t *) svc_handler,        /* SVC handler */
    0,
    0,
    (uint32_t *) pendsv_handler,     /* pendsv handler */
    (uint32_t *) systick_handler,    /* systick handler */
    0,                               /* Unused */
    0,                               /* Unused */
    0,                               /* Unused */
    0,                               /* Unused */
    0,                               /* Unused */
    0,                               /* Unused */
    0,                               /* Unused */
    0,                               /* Unused */
    0,                               /* Unused */
    0,                               /* Unused */
    0,                               /* Unused */
    0,                               /* Unused */
    0,                               /* Unused */
    0,                               /* Unused */
    0,                               /* Unused */
    0,                               /* Unused */
    0,                               /* Unused */
    0,                               /* Unused */
    0,                               /* Unused */
    0,                               /* Unused */
    0,                               /* Unused */
    0,                               /* Unused */
    0,                               /* Unused */
    0,                               /* Unused */
    0,                               /* Unused */
    0,                               /* Unused */
    0,                               /* Unused */
    0,                               /* Unused */
    0,                               /* Unused */
    0,                               /* Unused */
    0,                               /* Unused */
    0,                               /* Unused */
    0,                               /* Unused */
    0,                               /* Unused */
    0,                               /* Unused */
    0,                               /* Unused */
    0,                               /* Unused */
    (uint32_t *) USART1_IRQHandler   /* USART1 handler */
};

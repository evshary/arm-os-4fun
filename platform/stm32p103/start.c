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

void reset_handler(void)
{
    uint32_t *flash_data_begin = &_flash_start_data;
    uint32_t *data_begin = &_start_data;
    uint32_t *data_end = &_end_data;
    while (data_begin < data_end) *data_begin++ = *flash_data_begin++;

    uint32_t *bss_begin = &_start_bss;
    uint32_t *bss_end = &_end_bss;
    while (bss_begin < bss_end) *bss_begin++ = 0;

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
void USART2_IRQHandler();

__attribute((section(".init_isr")))
uint32_t *isr_vectors[] = {
    (uint32_t *) &_end_stack,        /* stack pointer */
    (uint32_t *) reset_handler,      /* code entry point */
    (uint32_t *) nmi_handler,        /* NMI handler */
    (uint32_t *) hardfault_handler,  /* hard fault handler */
    (uint32_t *) memfault_handler,   /* memory fault handler */
    (uint32_t *) busfault_handler,   /* bus fault handler */
    (uint32_t *) usagefault_handler, /* usage fault handler */
    0,                               /* Reserved */
    0,                               /* Reserved */
    0,                               /* Reserved */
    0,                               /* Reserved */
    (uint32_t *) svc_handler,        /* SVC handler */
    0,                               /* Debug Monitor */
    0,                               /* Reserved */
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
    0,                               /* Unused */
    (uint32_t *) USART2_IRQHandler   /* USART2 handler */
};

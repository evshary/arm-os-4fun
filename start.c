#include <stdint.h>
#include "reg.h"

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
    while (1);
}

void hardfault_handler(void)
{
    while (1);
}

void svc_handler(void);

__attribute((section(".init_isr")))
uint32_t *isr_vectors[] = {
    (uint32_t *) &_end_stack,        /* stack pointer */
    (uint32_t *) reset_handler,      /* code entry point */
    (uint32_t *) nmi_handler,        /* NMI handler */
    (uint32_t *) hardfault_handler,  /* hard fault handler */
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    (uint32_t *) svc_handler         /* SVC handler */
};

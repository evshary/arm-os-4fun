#include <stdint.h>
#include "reg.h"

/* 72MHz */
#define CPU_CLOCK_HZ 72000000
/* 100 ms per tick. */
#define TICK_RATE_HZ 10

unsigned int uptime;

void systick_handler(void)
{
    *SCB_ICSR |= SCB_ICSR_PENDSVSET;
    uptime++;
}

void systick_init(void)
{
    /* SysTick configuration */
    *SYSTICK_LOAD = (CPU_CLOCK_HZ / TICK_RATE_HZ) - 1UL;
    *SYSTICK_VAL = 0;
    *SYSTICK_CTRL = 0x07;
    uptime = 0;
}

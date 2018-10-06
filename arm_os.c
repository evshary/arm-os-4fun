#include <stdint.h>
#include "reg.h"
#include "output.h"
#include "tasks.h"
#include "syscall.h"
#include "malloc.h"

/* 72MHz */
#define CPU_CLOCK_HZ 72000000
/* 100 ms per tick. */
#define TICK_RATE_HZ 10

static char greet[] = "Hi, This is arm-os-4fun!\n";

void proc1(void)
{
    int id, priority;
    printfmt("USER: This is process 1\r\n");
    id = get_taskid();
    printfmt("USER: task id=%d\r\n", id);
    printfmt("USER: Back to process 1\r\n");
    priority = get_priority();
    printfmt("USER: priority=%d\r\n", priority);
    while (1);
}

void proc2(void)
{
    int id, priority;
    printfmt("USER: This is process 2\r\n");
    id = get_taskid();
    printfmt("USER: task id=%d\r\n", id);
    printfmt("USER: Back to process 2\r\n");
    priority = get_priority();
    printfmt("USER: priority=%d\r\n", priority);
    while (1);
}

void main(void)
{
    int proc_id[USER_PROCESS];
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

    /* SysTick configuration */
    *SYSTICK_LOAD = (CPU_CLOCK_HZ / TICK_RATE_HZ) - 1UL;
    *SYSTICK_VAL = 0;
    *SYSTICK_CTRL = 0x07;

    printfmt(greet);
    malloc_init();
    tasks_init();
    proc_id[0] = new_task(proc1, 1);
    printfmt("proc_id[0]=%d\r\n", proc_id[0]);
    proc_id[1] = new_task(proc2, 2);
    printfmt("proc_id[1]=%d\r\n", proc_id[1]);
    while (1) {
        tasks_scheduler();
    }
    while (1);
}

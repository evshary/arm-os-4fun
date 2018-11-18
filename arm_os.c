#include <stdint.h>
#include "uart.h"
#include "systick.h"
#include "output.h"
#include "tasks.h"
#include "syscall.h"
#include "malloc.h"

static char greet[] = "Hi, This is arm-os-4fun!\r\n";

void proc1(void)
{
    int id, priority, exetime;
    printfmt("USER: This is process 1\r\n");
    id = get_taskid();
    printfmt("USER: task id=%d\r\n", id);
    printfmt("USER: Back to process 1\r\n");
    priority = get_priority();
    printfmt("USER: priority=%d\r\n", priority);
    exetime = get_exetime();
    printfmt("USER: exetime=%d\r\n", exetime);

    while (1) {
        char buf[10];
        int ret = read(buf, 10);
        printfmt("buf[0]=%c ret=%d\r\n", buf[0], ret);
    }
}

void proc2(void)
{
    int id, priority, exetime;
    printfmt("USER: This is process 2\r\n");
    id = get_taskid();
    printfmt("USER: task id=%d\r\n", id);
    printfmt("USER: Back to process 2\r\n");
    priority = get_priority();
    printfmt("USER: priority=%d\r\n", priority);
    exetime = get_exetime();
    printfmt("USER: exetime=%d\r\n", exetime);
    while (1);
}

void main(void)
{
    int proc_id[USER_PROCESS];

    uart_init();
    systick_init();

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

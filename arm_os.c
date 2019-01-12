#include <stdint.h>
#include "uart.h"
#include "systick.h"
#include "output.h"
#include "tasks.h"
#include "syscall.h"
#include "malloc.h"

static char greet[] = "Hi, This is arm-os-4fun!\r\n";

void dummy_proc(void)
{
    while (1);
}

void proc1(void)
{
    int id, priority, exetime;
    printfmt("USER1: This is process 1\r\n");
    id = get_taskid();
    printfmt("USER1: task id=%d\r\n", id);
    priority = get_priority();
    printfmt("USER1: priority=%d\r\n", priority);
    exetime = get_exetime();
    printfmt("USER1: exetime=%d\r\n", exetime);

    while (1) {
        char buf[10];
        int ret = read(buf, 10);
        printfmt("buf[0]=%c ret=%d\r\n", buf[0], ret);
        sleep(1);
    }
}

void proc2(void)
{
    int id, priority, exetime;
    printfmt("USER2: This is process 2\r\n");
    id = get_taskid();
    printfmt("USER2: task id=%d\r\n", id);
    priority = get_priority();
    printfmt("USER2: priority=%d\r\n", priority);
    exetime = get_exetime();
    printfmt("USER2: exetime=%d\r\n", exetime);
    while (1) {
        printfmt("USER2: Infinite Sleep\r\n");
        sleep(1);
    }
}

void main(void)
{
    int proc_id[USER_PROCESS];

    uart_init();
    systick_init();

    printfmt(greet);
    malloc_init();
    tasks_init();
    /*
     * We should start a dummy_proc to prevent no running process.
     * This will cause infinite loop in kernel(Always in handler mode).
     */
    proc_id[0] = new_task(dummy_proc, 0);
    /* Create your own process */
    proc_id[1] = new_task(proc1, 1);
    printfmt("proc_id[1]=%d\r\n", proc_id[1]);
    proc_id[2] = new_task(proc2, 2);
    printfmt("proc_id[2]=%d\r\n", proc_id[2]);
    while (1) {
        tasks_scheduler();
    }
    while (1); /* You should not enter here. */
}

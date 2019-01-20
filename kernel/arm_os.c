#include <stdint.h>
#include "usart.h"
#include "systick.h"
#include "output.h"
#include "tasks.h"
#include "syscall.h"
#include "malloc.h"
#include "common.h"

static char greet[] = "Hi, This is arm-os-4fun!\r\n";

void dummy_task(void)
{
    while (1);
}

void task1(void)
{
    int id, priority, exetime;
    printfmt("USER1: This is task 1\r\n");
    id = get_taskid();
    printfmt("USER1: task id=%d\r\n", id);
    priority = get_priority();
    printfmt("USER1: priority=%d\r\n", priority);
    exetime = get_exetime();
    printfmt("USER1: exetime=%d\r\n", exetime);

    while (1) {
        char buf[10];
        memset(buf, 0, sizeof(buf));
        int ret = read(buf, 10);
        printfmt("buf=%s ret=%d\r\n", buf, ret);
        sleep(1);
    }
}

void task2(void)
{
    int id, priority, exetime;
    printfmt("USER2: This is task 2\r\n");
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
    int task_id[MAX_TASK_NUM];

    uart_init();
    systick_init();

    printfmt(greet);
    malloc_init();
    tasks_init();
    /*
     * We should start a dummy_task to prevent no running task.
     * This will cause infinite loop in kernel(Always in handler mode).
     */
    task_id[0] = new_task(dummy_task, 0);
    /* Create your own task */
    task_id[1] = new_task(task1, 1);
    printfmt("task_id[1]=%d\r\n", task_id[1]);
    task_id[2] = new_task(task2, 2);
    printfmt("task_id[2]=%d\r\n", task_id[2]);
    while (1) {
        tasks_scheduler();
    }
    while (1); /* You should not enter here. */
}

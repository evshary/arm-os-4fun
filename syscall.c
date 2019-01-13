#include "syscall.h"
#include "tasks.h"
#include "output.h"

int get_taskid(void)
{
    int taskid;
    task_syscall_setparam(SYSCALL_GET_TASKID, 0);
    task_syscall_setretval(&taskid);
    __asm__
    (
        "svc 0"
    );
    return taskid;
}

int get_priority(void)
{
    int priority;
    task_syscall_setparam(SYSCALL_GET_PRIORITY, 0);
    task_syscall_setretval(&priority);
    __asm__
    (
        "svc 0"
    );
    return priority;
}

int get_exetime(void)
{
    int exetime;
    task_syscall_setparam(SYSCALL_GET_EXETIME, 0);
    task_syscall_setretval(&exetime);
    __asm__
    (
        "svc 0"
    );
    return exetime;
}

int read(char *buf, int len)
{
    int read_len;
    struct buf_struct buf_ptr;
    buf_ptr.buf = buf;
    buf_ptr.len = len;

    task_syscall_setparam(SYSCALL_READ, &buf_ptr);
    task_syscall_setretval(&read_len);
    __asm__
    (
        "svc 0"
    );
    return read_len;
}

void sleep(unsigned int time)
{
    task_syscall_setparam(SYSCALL_SLEEP, &time);
    __asm__
    (
        "svc 0"
    );
    return;
}

int get_cpu_usage(void)
{
    int cpu_usage;
    task_syscall_setparam(SYSCALL_GET_CPU_USAGE, 0);
    task_syscall_setretval(&cpu_usage);
    __asm__
    (
        "svc 0"
    );
    return cpu_usage;
}

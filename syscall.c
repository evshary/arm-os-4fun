#include "syscall.h"
#include "tasks.h"
#include "output.h"

int get_taskid(void)
{
    void *ret_val;
    task_syscall_setparam(SYSCALL_GET_TASKID, 0);
    __asm__
    (
        "svc 0"
    );
    ret_val = task_syscall_getretval();
    return *((int *)ret_val);
}

int get_priority(void)
{
    void *ret_val;
    task_syscall_setparam(SYSCALL_GET_PRIORITY, 0);
    __asm__
    (
        "svc 0"
    );
    ret_val = task_syscall_getretval();
    return *((int *)ret_val);
}

int get_exetime(void)
{
    void *ret_val;
    task_syscall_setparam(SYSCALL_GET_EXETIME, 0);
    __asm__
    (
        "svc 0"
    );
    ret_val = task_syscall_getretval();
    return *((int *)ret_val);
}

int read(char *buf, int len)
{
    int ret_val;
    struct buf_struct buf_ptr;
    buf_ptr.buf = buf;
    buf_ptr.len = len;

    task_syscall_setparam(SYSCALL_READ, &buf_ptr);
    __asm__
    (
        "svc 0"
    );
    ret_val = (int)task_syscall_getretval();
    return ret_val;
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

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

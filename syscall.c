#include "syscall.h"
#include "tasks.h"

int cur_syscall_num;
void *cur_syscall_param;
void *ret_val;

int get_taskid(void)
{
    cur_syscall_num = SYSCALL_GET_TASKID;
    cur_syscall_param = 0;
    __asm__
    (
        "svc 0"
    );
    cur_syscall_num = SYSCALL_INIT;
    cur_syscall_param = 0;
    return *((int *)ret_val);
}

void second_call(void)
{
    cur_syscall_num = SYSCALL_SECOND_CALL;
    cur_syscall_param = 0;
    __asm__
    (
        "svc 0"
    );
    cur_syscall_num = SYSCALL_INIT;
    cur_syscall_param = 0;
}

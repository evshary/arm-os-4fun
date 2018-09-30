#include "tasks.h"
#include "syscall.h"
#include "output.h"

#define STACK_SIZE 512

unsigned int *run_proc(unsigned int *stack);

unsigned int user_stack[USER_PROCESS][STACK_SIZE];
struct task_control_block tasks[USER_PROCESS];

void tasks_init(void)
{
    int i;
    for (i = 0; i < USER_PROCESS; i++) {
        tasks[i].id = 0;
        tasks[i].status = 0;
        tasks[i].priority = 0;
        tasks[i].time = 0;
        tasks[i].syscall_num = 0;
        tasks[i].syscall_param = 0;
        tasks[i].user_stack_ptr = 0;
    }
}

int new_task(void *proc_addr)
{
    unsigned int available_id = 0;
    int i;

    /* Select empty tasks */
    for (i = 0; i < USER_PROCESS; i++) {
        if (tasks[i].id == 0) {
            available_id = i;
            break;
        }
    }
    /* Not enough space for new process */
    if (i >= USER_PROCESS)
        return -1;
    /*
     * We will pop the regiser with the following order, r4-r11, lr
     * The reason we reserve 17 is that psr, pc, lr, r12, r3-r0
     * will be saved by ARM automatically.
     * We save 9 register and ARM save 8 register, so total is 17.
     * Ref: http://www.360doc.com/content/12/0506/11/532901_208997264.shtml
     */
    tasks[available_id].user_stack_ptr = &user_stack[available_id][STACK_SIZE - 17];
    /*
     * While lr=0xfffffffd, ARM will go to user thread mode, change sp to psp,
     * and then pop the registers (par, pc, lr, r12, r3-r0) out from psp.
     */
    /*
     * We need to use lr=0xfffffffd, that is EXC_RETURN, to return back
     * to user thread mode. Control register can only make us enter user
     * thread mode from privileged thread mode, not privileged handler mode.
     * Therefore, if we don't use EXC_RETURN, it will cause problem while
     * calling "svc" in process 2, because svc can only be called in
     * user thread mode but we are in privileged handler mode now.
     * Control register just change stack from msp to psp but not from
     * privileged handler mode to user thread mode.
     */
    tasks[available_id].user_stack_ptr[8] = (unsigned int)0xFFFFFFFD;
    /* It's necessary to init lr with process address first. */
    tasks[available_id].user_stack_ptr[15] = (unsigned int)proc_addr;
    /* PSR Thumb bit */
    tasks[available_id].user_stack_ptr[16] = (unsigned int)0x01000000;
    available_id++;
    return available_id;
}

void start_process(int id)
{
    tasks[id - 1].user_stack_ptr = run_proc(tasks[id - 1].user_stack_ptr);
    if (cur_syscall_num > 0) {
        tasks[id - 1].syscall_num = cur_syscall_num;
        tasks[id - 1].syscall_param = cur_syscall_param;
        printfmt("syscall_num=%d\r\n", tasks[id - 1].syscall_num);
        /* Handling the system call */
        switch (tasks[id - 1].syscall_num) {
            case SYSCALL_FIRST_CALL:
                printfmt("This is first call\r\n");
                break;
            case SYSCALL_SECOND_CALL:
                printfmt("This is second call\r\n");
                break;
            default:
                printfmt("Unsupported syscall num\r\n");
        }
    }
}


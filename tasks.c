#include "tasks.h"
#include "syscall.h"
#include "output.h"

#define TASK_NONE      0
#define TASK_READY     1
#define TASK_BLOCK     2
#define TASK_RUNNING   3

#define STACK_SIZE 512

unsigned int *run_proc(unsigned int *stack);

unsigned int user_stack[USER_PROCESS][STACK_SIZE];
struct task_control_block tasks[USER_PROCESS];
unsigned char current_task_id;

void tasks_init(void)
{
    int i;
    for (i = 0; i < USER_PROCESS; i++) {
        tasks[i].id = 0;
        tasks[i].status = TASK_NONE;
        tasks[i].priority = 0;
        tasks[i].time = 0;
        tasks[i].syscall_num = 0;
        tasks[i].syscall_param = 0;
        tasks[i].user_stack_ptr = 0;
    }
    current_task_id = -1;
}

int new_task(void *proc_addr, int priority)
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

    /* Update the task member */
    tasks[available_id].id = available_id + 1;
    tasks[available_id].priority = priority;
    tasks[available_id].status = TASK_READY;

    available_id++;
    return available_id;
}

void tasks_scheduler(void)
{
    int i, id;

    /* Select available tasks */
    /* Round robin scheduler */
    for (i = 1; i <= USER_PROCESS; i++) {
        if (tasks[(current_task_id + i) % USER_PROCESS].status == TASK_READY) {
            id = (current_task_id + i) % USER_PROCESS;
            break;
        }
    }
    printfmt("Now we want to run id=%d\r\n", id);
    tasks[id].status = TASK_RUNNING;
    current_task_id = id;

    tasks[id].user_stack_ptr = run_proc(tasks[id].user_stack_ptr);
    if (cur_syscall_num > 0) {
        tasks[id].syscall_num = cur_syscall_num;
        tasks[id].syscall_param = cur_syscall_param;
        printfmt("syscall_num=%d\r\n", tasks[id].syscall_num);
        /* Handling the system call */
        switch (tasks[id].syscall_num) {
            case SYSCALL_GET_TASKID:
                printfmt("Task ID = %d\r\n", tasks[id].id);
                ret_val = &tasks[id].id;
                break;
            case SYSCALL_GET_PRIORITY:
                printfmt("Priority = %d\r\n", tasks[id].priority);
                ret_val = &tasks[id].priority;
                break;
            default:
                printfmt("Unsupported syscall num\r\n");
        }
    }
    tasks[id].status = TASK_READY;
}


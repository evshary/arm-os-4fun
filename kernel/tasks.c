#include "tasks.h"
#include "syscall.h"
#include "output.h"

#define TASK_NONE      0
#define TASK_READY     1
#define TASK_BLOCK     2
#define TASK_RUNNING   3

#define STACK_SIZE 512

#define TASK_DEBUG 0
#if TASK_DEBUG
#define TASK_DBG printfmt
#else
#define TASK_DBG(...)
#endif

unsigned int *run_task(unsigned int *stack);
unsigned int *switch_to_handler(unsigned int *stack);

unsigned int user_stack[MAX_TASK_NUM][STACK_SIZE];
struct task_control_block tasks[MAX_TASK_NUM];
unsigned char current_task_id;

extern unsigned int uptime;

void tasks_init(void)
{
    int i;
    unsigned int empty_stack[32];

    /* We use empty stack to switch CPU mode to handler mode.
     * The reason we need to switch to handler mode is that
     * ARM CM3/4 is unable to use EXC_RETURN in thread mode.
     * Ref1: https://github.com/jserv/mini-arm-os/issues/27
     * Ref2: https://www.ptt.cc/bbs/ASM/M.1534260340.A.DC5.html
     */
    switch_to_handler(&empty_stack[32]);
    for (i = 0; i < MAX_TASK_NUM; i++) {
        tasks[i].id = 0;
        tasks[i].status = TASK_NONE;
        tasks[i].priority = 0;
        tasks[i].exe_time = 0;
        tasks[i].restart_time = 0;
        tasks[i].syscall_num = 0;
        tasks[i].syscall_param = 0;
        tasks[i].user_stack_ptr = 0;
    }
    current_task_id = -1;
}

int new_task(void *task_addr, int priority)
{
    unsigned int available_id = 0;
    int i;

    /* Select empty task */
    for (i = 0; i < MAX_TASK_NUM; i++) {
        if (tasks[i].id == 0) {
            available_id = i;
            break;
        }
    }
    /* Not enough space for new task */
    if (i >= MAX_TASK_NUM)
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
    tasks[available_id].user_stack_ptr[8] = (unsigned int)0xFFFFFFFD;
    /* It's necessary to init lr with task address first. */
    tasks[available_id].user_stack_ptr[15] = (unsigned int)task_addr;
    /* PSR Thumb bit */
    tasks[available_id].user_stack_ptr[16] = (unsigned int)0x01000000;

    /* Update the task member */
    tasks[available_id].id = available_id + 1;
    tasks[available_id].priority = priority;
    tasks[available_id].status = TASK_READY;

    available_id++;
    return available_id;
}

void task_syscall_setparam(int syscall_num, void *param)
{
    tasks[current_task_id].syscall_num = syscall_num;
    tasks[current_task_id].syscall_param = param;
}

void task_syscall_setretval(void *retval)
{
    tasks[current_task_id].syscall_retval = retval;
}

void tasks_scheduler(void)
{
    unsigned int last_time;
    int i, id;

    /* Update the status of tasks */
    for (i = 1; i < MAX_TASK_NUM; i++) {
        if (tasks[i].status == TASK_BLOCK && tasks[i].restart_time <= uptime) {
            tasks[i].status = TASK_READY;
            tasks[i].restart_time = 0;
        }
    }

    /* Select available tasks */
    /* Round robin scheduler */
    for (i = 1; i <= MAX_TASK_NUM; i++) {
        /* We can run dummy_task only when no ready task */
        if ((current_task_id + i) % MAX_TASK_NUM == 0) {
            id = 0;
            continue;
        }
        if (tasks[(current_task_id + i) % MAX_TASK_NUM].status == TASK_READY) {
            id = (current_task_id + i) % MAX_TASK_NUM;
            break;
        }
    }

    TASK_DBG("KERNEL: Now we want to run id=%d\r\n", id);
    tasks[id].status = TASK_RUNNING;
    current_task_id = id;
    last_time = uptime;

    tasks[id].user_stack_ptr = run_task(tasks[id].user_stack_ptr);
    TASK_DBG("KERNEL: uptime=%d, last_time=%d\r\n", uptime, last_time);
    tasks[id].exe_time += uptime - last_time;
    if (tasks[id].syscall_num > 0) {
        TASK_DBG("KERNEL: syscall_num=%d\r\n", tasks[id].syscall_num);
        /* Handling the system call */
        switch (tasks[id].syscall_num) {
            case SYSCALL_GET_TASKID:
                TASK_DBG("KERNEL: Task ID = %d\r\n", tasks[id].id);
                *(int *)tasks[id].syscall_retval = tasks[id].id;
                break;
            case SYSCALL_GET_PRIORITY:
                TASK_DBG("KERNEL: Priority = %d\r\n", tasks[id].priority);
                *(int *)tasks[id].syscall_retval = tasks[id].priority;
                break;
            case SYSCALL_GET_EXETIME:
                TASK_DBG("KERNEL: EXETIME = %d\r\n", tasks[id].exe_time);
                *(int *)tasks[id].syscall_retval = tasks[id].exe_time;
                break;
            case SYSCALL_READ: {
                struct buf_struct *buf_ptr = tasks[id].syscall_param;
                extern char tmp_ch;
                TASK_DBG("KERNEL: READ buf=%x len=%d\r\n", buf_ptr->buf, buf_ptr->len);
                /* get data from USART2 */
                buf_ptr->buf[0] = tmp_ch;
                tasks[id].syscall_retval = (void *)1;
                break;
            }
            case SYSCALL_SLEEP:
                TASK_DBG("KERNEL: SLEEP = %d\r\n", *(int *)tasks[id].syscall_param);
                tasks[id].restart_time = uptime + *(int *)tasks[id].syscall_param;
                tasks[id].status = TASK_BLOCK;
                break;
            case SYSCALL_GET_CPU_USAGE: {
                int *retval = tasks[id].syscall_retval;
                *retval = 100 - ((tasks[0].exe_time * 100) / uptime);
                TASK_DBG("KERNEL: CPU Usage = %d\r\n", *retval);
                break;
            }
            default:
                TASK_DBG("KERNEL: Unsupported syscall num\r\n");
                break;
        }
        tasks[id].syscall_num = SYSCALL_INIT;
        tasks[id].syscall_param = 0;
    }
    if (tasks[id].status == TASK_RUNNING)
        tasks[id].status = TASK_READY;
}


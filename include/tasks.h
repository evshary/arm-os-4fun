#ifndef __TASKS_H__
#define __TASKS_H__

#define MAX_TASK_NUM 32

struct task_control_block {
    unsigned char status;       /* Task status */
    unsigned int  id;           /* Task id */
    unsigned int  priority;     /* Task priority */
    unsigned int  exe_time;     /* The execution time*/
    unsigned int  restart_time; /* Record the time to restart task */
    int syscall_num;
    void *syscall_param;
    void *syscall_retval;
    unsigned int *user_stack_ptr;
};

void tasks_init(void);
int new_task(void *task_addr, int priority);
void task_syscall_setparam(int syscall_num, void *param);
void task_syscall_setretval(void *retval);
void tasks_scheduler(void);

extern unsigned char current_task_id;

#endif /*__TASKS_H__*/

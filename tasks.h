#ifndef __TASKS_H__
#define __TASKS_H__

#define USER_PROCESS 32

struct task_control_block {
    unsigned char id;       /* Task id */
    unsigned char status;   /* Task state */
    unsigned char priority; /* Task priority */
    unsigned int time;      /* The execution time*/
    unsigned int syscall_num;
    void *syscall_param;
    unsigned int *user_stack_ptr;
};

void tasks_init(void);
int new_task(void *proc_addr);
void start_process(int id);

#endif /*__TASKS_H__*/

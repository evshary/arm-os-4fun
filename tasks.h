#ifndef __TASKS_H__
#define __TASKS_H__

#define USER_PROCESS 32

struct task_type {
    unsigned int syscall_num;
    void *syscall_param;
    unsigned int *user_stack_ptr;
};

int init_process(void *proc_addr);
void start_process(int id);

#endif /*__TASKS_H__*/

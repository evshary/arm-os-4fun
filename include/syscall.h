#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#define SYSCALL_INIT          -1
#define SYSCALL_GET_TASKID     1
#define SYSCALL_GET_PRIORITY   2
#define SYSCALL_GET_EXETIME    3

extern int cur_syscall_num;
extern void *cur_syscall_param;
extern void *ret_val;

int get_taskid(void);
int get_priority(void);
int get_exetime(void);

#endif /*__SYSCALL_H__*/
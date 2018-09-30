#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#define SYSCALL_INIT         -1
#define SYSCALL_GET_TASKID    1
#define SYSCALL_SECOND_CALL   2

extern int cur_syscall_num;
extern void *cur_syscall_param;
extern void *ret_val;

int get_taskid(void);
void second_call(void);

#endif /*__SYSCALL_H__*/

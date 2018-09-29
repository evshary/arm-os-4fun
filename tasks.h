#ifndef __TASKS_H__
#define __TASKS_H__

#define USER_PROCESS 32

int init_process(void *proc_addr);
void start_process(int id);

#endif /*__TASKS_H__*/

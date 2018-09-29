#include "tasks.h"

#define STACK_SIZE 512

unsigned int *run_proc(unsigned int *stack);

unsigned int  user_stack[USER_PROCESS][STACK_SIZE];
unsigned int* user_stack_ptr[USER_PROCESS];
unsigned int  current_proc_id = 0;

int init_process(void *proc_addr)
{
    /*
     * We will pop the regiser with the following order, r4-r11, lr
     * The reason we reserve 17 is that psr, pc, lr, r12, r3-r0
     * will be saved by ARM automatically.
     * We save 9 register and ARM save 8 register, so total is 17.
     * Ref: http://www.360doc.com/content/12/0506/11/532901_208997264.shtml
     */
    user_stack_ptr[current_proc_id] = &user_stack[current_proc_id][STACK_SIZE - 17];
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
    user_stack_ptr[current_proc_id][8] = (unsigned int)0xFFFFFFFD;
    /* It's necessary to init lr with process address first. */
    user_stack_ptr[current_proc_id][15] = (unsigned int)proc_addr;
    /* PSR Thumb bit */
    user_stack_ptr[current_proc_id][16] = (unsigned int)0x01000000;
    current_proc_id++;
    return current_proc_id;
}

void start_process(int id)
{
    user_stack_ptr[id - 1] = run_proc(user_stack_ptr[id - 1]);
}


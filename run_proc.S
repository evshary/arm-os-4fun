.thumb
.syntax unified

.global switch_to_handler
switch_to_handler:
    /* Save psr to r12 and save kernel's register to stack(msp) */
    mrs r12, psr
    push {r4-r12, lr}

    /* Switch to unprivileged thread mode. */
    msr psp, r0
    mov r0, #3
    msr control, r0
    /* isb means Instruction Synchronization Barrier */
    /* This makes sure the following command should be in correct privilege. */
    /* ref: http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dai0321a/BIHFJCAC.html */
    isb

    /* Switch to handler mode */
    svc 0

    bx lr

.global run_proc 
run_proc:
    /* Save psr to r12 and save kernel's register to stack(msp) */
    mrs r12, psr
    push {r4-r12, lr}
    
    /* Load saved register from user task */
    ldmia r0!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    /* Setup psp */
    msr psp, r0

    /* Use EXC_RETURN to jump to user task */
    bx lr

/* 
 * If we don't define svc_handler as a function type, 
 * the ISR in vector table will not be the address of 
 * svc_handler + 1.
 */
.type svc_handler, %function
.global svc_handler
/* 
 * For pendsv and svc, they are both back to kernel. 
 * we can put them together.
 * See https://blog.csdn.net/guozhongwei1/article/details/49544671 
 * to realize the reason why we should use pendsv.
 */
.type pendsv_handler, %function
.global pendsv_handler
svc_handler:
pendsv_handler:
    /* Now stack is msp, so we can't use push to save register. */
    /* r0 register is necessary, because r0 stands for return value and we should return stack address. */
    mrs r0, psp
    stmdb r0!, {r4-r11, lr}

    /* Restore the register of kernel state */
    pop {r4-r12, lr}
    msr psr_nzcvq, r12

    /* Jump back to kernel */
    bx lr

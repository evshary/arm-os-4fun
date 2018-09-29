.thumb
.syntax unified

.global run_proc 
run_proc:
    /* Save psr to r12 and push to stack, msp */
    mrs r12, psr
    push {r4-r12, lr}
    
    /* put param1 to psp and jump to user mode */
    msr psp, r0
    mov r0, #3
    msr control, r0
    /* isb means Instruction Synchronization Barrier */
    /* This makes sure the following command should be in correct privilege. */
    /* ref: http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dai0321a/BIHFJCAC.html */
    isb

    /* pop the register and process address from psp */
    pop {r4-r11, lr}
    bx lr

/* 
 * If we don't define svc_handler as a function type, 
 * the ISR in vector table will not be the address of 
 * svc_handler + 1.
 */
.type svc_handler, %function
.global svc_handler
/* 
 * For systick and svc, they are both back to kernel. 
 * we can put them together.
 */
.type systick_handler, %function
.global systick_handler
svc_handler:
systick_handler:
    /* Now stack is msp, so we can't use push to save register. */
    /* r0 register is necessary, because r0 stands for return value and we should return stack address. */
    mrs r0, psp
    stmdb r0!, {r4-r11, lr}

    /* Restore the register of kernel state */
    pop {r4-r12, lr}
    msr psr_nzcvq, r12

    /* Jump back to kernel */
    bx lr

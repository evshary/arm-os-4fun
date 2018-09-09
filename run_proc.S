.thumb
.syntax unified

.global run_proc 
run_proc:
    mrs r12, psr
    push {r4-r12, lr}
    
    msr psp, r0
    mov r0, #3
    msr control, r0

    pop {r4-r12, lr}
    bx lr

/* 
 * If we don't define svc_handler as a function type, 
 * the ISR in vector table will not be the address of 
 * svc_handler + 1.
 */
.type svc_handler, %function
.global svc_handler
svc_handler:
    /* Now stack is msp, so we can't use push to save register. */
    mrs r0, psp
    stmdb r0!, {r4-r12, lr}

    /* Restore the register of kernel state */
    pop {r4-r12, lr}
    msr psr_nzcvq, r12

    /* Jump back to kernel */
    bx lr

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

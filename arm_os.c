#include <stdint.h>
#include <stdarg.h>
#include "reg.h"

#define USART_FLAG_TXE  ((uint16_t) 0x0080)
#define USER_PROCESS 32
#define STACK_SIZE   512

static char greet[] = "Hi, This is arm-os-4fun!\n";
unsigned int *run_proc(unsigned int *stack);
void print_char(char ch);
void print_str(const char *str);
void printfmt(char *fmt, ...);
void print_stack(unsigned int *stack, int size);

unsigned int  user_stack[USER_PROCESS][STACK_SIZE];
unsigned int* user_stack_ptr[USER_PROCESS];
unsigned int  current_proc_id = 0;

void print_char(char ch)
{
    while (!(*(USART2_SR) & USART_FLAG_TXE));
    *(USART2_DR) = (ch & 0xFF);
}

void print_str(const char *str)
{
    while (*str) {
        print_char(*str);
        str++;
    }
}

char *cvt_int(unsigned int num, int base)
{
    static char repr[] = "0123456789abcdef";
    static char buffer[50];
    char *ptr;

    ptr = &buffer[49];
    *ptr = '\0';

    do {
        *--ptr = repr[num % base];
        num /= base;
    } while (num != 0);

    return (ptr);
}

void printfmt(char *fmt, ...)
{
    char *ptr;
    int i;
    char *s;
    va_list arg;
    va_start(arg, fmt);

    for (ptr = fmt; *ptr != 0; ptr++) {
        if (*ptr != '%') {
            print_char(*ptr);
            continue;
        }
        ptr++;
        switch (*ptr) {
            case 'c':
                i = va_arg(arg, int);
                print_char(i);
                break;
            case 'd':
                i = va_arg(arg, int);
                if (i < 0) {
                    i = -i;
                    print_char('-');
                }
                print_str(cvt_int(i, 10));
                break;
            case 's':
                s = va_arg(arg, char *);
                print_str(s);
                break;
            case 'x':
                i = va_arg(arg, unsigned int);
                print_str(cvt_int(i, 16));
                break;
        }
    }
    va_end(arg);
}

void print_stack(unsigned int *stack, int size)
{
    int i;
    printfmt("Start to print stack %x\r\n", stack);
    for (i = 0; i < size; i++) {
        printfmt("%x\r\n", stack[i]);
    }
}

void first_call(void)
{
    __asm__
    (
        "svc 0"
    );
}

void proc1(void)
{
    printfmt("This is process 1\r\n");
    printfmt("user_stack_ptr[0][9]=%x\r\n", user_stack_ptr[0][9]);
    first_call();
    printfmt("Back to process 1\r\n");
    while (1);
}

void proc2(void)
{
    printfmt("This is process 2\r\n");
    printfmt("user_stack_ptr[1][9]=%x\r\n", user_stack_ptr[1][9]);
    first_call();
    printfmt("Back to process 2\r\n");
    while (1);
}

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
     * While lr=0xfffffffd, ARM will go to thread mode, change sp to psp,
     * and then pop the registers (par, pc, lr, r12, r3-r0) out from psp.
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

void main(void)
{
    int proc_id[USER_PROCESS];
    *(RCC_APB2ENR) |= (uint32_t)(0x00000001 | 0x00000004);
    *(RCC_APB1ENR) |= (uint32_t)(0x00020000);

    *(GPIOA_CRL) = 0x00004B00;
    *(GPIOA_CRH) = 0x44444444;
    *(GPIOA_ODR) = 0x00000000;
    *(GPIOA_BSRR) = 0x00000000;
    *(GPIOA_BRR) = 0x00000000;

    *(USART2_CR1) = 0x0000000C;
    *(USART2_CR2) = 0x00000000;
    *(USART2_CR3) = 0x00000000;
    *(USART2_CR1) |= 0x2000;

    printfmt(greet);
    proc_id[0] = init_process(proc1);
    printfmt("proc_id[0]=%d\r\n", proc_id[0]);
    proc_id[1] = init_process(proc2);
    printfmt("proc_id[1]=%d\r\n", proc_id[1]);
    start_process(proc_id[0]);
    printfmt("Return from process 1\r\n");
    start_process(proc_id[1]);
    printfmt("Return from process 2\r\n");
    start_process(proc_id[0]);
    while (1);
}

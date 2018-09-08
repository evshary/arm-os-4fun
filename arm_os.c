#include <stdint.h>
#include "reg.h"

#define USART_FLAG_TXE  ((uint16_t) 0x0080)
#define USER_PROCESS 32
#define STACK_SIZE   512

static char greet[] = "Hello World!\n";
void run_proc(unsigned int *stack);

unsigned int user_stack[USER_PROCESS][STACK_SIZE];
unsigned int current_proc_id = 0;

void print(const char *str)
{
    while (*str) {
        while (!(*(USART2_SR) & USART_FLAG_TXE));
        *(USART2_DR) = (*str & 0xFF);
        str++;
    }
}

void proc1(void)
{
	print("This is process 1\r\n");
	while(1);
}

int init_process(void *proc_addr)
{
	/* 
     * We will pop the regiser with the following order, r4-r12, lr
     * It's necessary to init lr with process address first.
	 */
	user_stack[current_proc_id][STACK_SIZE-16+9] = (unsigned int)proc_addr;
	current_proc_id++;
	return current_proc_id;
}

void start_process(int id)
{
    run_proc(&user_stack[id-1][STACK_SIZE-16]);
}

void main(void)
{
    int proc_id;
    *(RCC_APB2ENR) |= (uint32_t) (0x00000001 | 0x00000004);
    *(RCC_APB1ENR) |= (uint32_t) (0x00020000);

    *(GPIOA_CRL) = 0x00004B00;
    *(GPIOA_CRH) = 0x44444444;
    *(GPIOA_ODR) = 0x00000000;
    *(GPIOA_BSRR) = 0x00000000;
    *(GPIOA_BRR) = 0x00000000;

    *(USART2_CR1) = 0x0000000C;
    *(USART2_CR2) = 0x00000000;
    *(USART2_CR3) = 0x00000000;
    *(USART2_CR1) |= 0x2000;

    print(greet);
    proc_id = init_process(proc1);
    start_process(proc_id);
    while(1);
}

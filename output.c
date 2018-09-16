#include <stdint.h>
#include <stdarg.h>
#include "output.h"
#include "reg.h"

#define USART_FLAG_TXE  ((uint16_t) 0x0080)

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
        printfmt("%x,", stack[i]);
        if (i && i % 10 == 0)
            printfmt("\r\n");
    }
    printfmt("\r\n");
}

void print_reg(void)
{
    register uint32_t reg_value;
    __asm__ volatile("mov %0, lr\n" : "=r"(reg_value));
    printfmt("lr=%x\r\n", reg_value);
    __asm__ volatile("mrs r0,control\nmov %0, r0\n" : "=r"(reg_value));
    printfmt("control=%x\r\n", reg_value);
}

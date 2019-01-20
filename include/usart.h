#ifndef __UART_H__
#define __UART_H__

void print_char(char ch);
int usart_read(char *str, int len);
void print_reg(void);
void uart_init(void);

#endif /*__UART_H__*/

#ifndef __UART_H_
#define __UART_H_

#include "type.h"
#include "io.h"
#include "s3c2440.h"

/* uart0,1,2 controller entry */
#define UART0_ENTRY 0x50000000
#define UART1_ENTRY 0x50004000
#define UART2_ENTRY 0x50008000

/* ports name */
#define UART0_PORT 0
#define UART1_PORT 1
#define UART2_PORT 2


void uart_init(u32 index);
void putc(const u32 port, const char ch);
void puts(const u32 port, const char *s);
char getc(const u32 port);

#endif /* __UART_H_ */
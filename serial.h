#ifndef SERIAL_H
#define SERIAL_H
#include <stdint.h>

void serial_init(void);
void serial_putc (const char a);
unsigned char serial_getc(void);
int serial_getc_5sec(unsigned char * a);
void serial_puts(const char * s );

#endif

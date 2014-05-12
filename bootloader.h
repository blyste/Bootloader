#ifndef BOOTLOADER_H
#define BOOTLOADER_H
#include <stdint.h>

void interface(void);
void go(uint32_t addr);

#endif

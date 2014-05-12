#ifndef LEDS_H
#define LEDS_H
#include <stdint.h>

void leds_init(void);
void led(int port, int num_led, int off);
void all_led(int off);

#endif

#include "registers.h"
#include "timers.h"
#include "leds.h"

extern int8_t _bss;
extern int8_t _ebss;

void initialisation(void){
	// Mise à zéro de la bss
	int8_t * i ;
	for ( i = &_bss ; i < &_ebss ; i++)
		*i = 0;
}

int main() {
	leds_init();
	timers_init();
	while(1){
		count_5sec();
		while(TCNTO1 !=0) {};
		all_led(0);
		count_5sec();
		while(TCNTO1 !=0) {};
		all_led(1);
	}
	return 0;
}


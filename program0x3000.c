#include "registers.h"
#include "timers.h"
#include "leds.h"

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


#include "serial.h"
#include "timers.h"
#include <stdint.h>
#include "bootloader.h"

int main() {
	serial_init();
	timers_init();
	interface();
	return 0;
}

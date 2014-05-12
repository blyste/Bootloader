#include "registers.h"
#include "leds.h"

void leds_init(void) {
	//Configure les registres des LEDS (en output)
	// Configuration de la led E5
	PCONE |= 1 << 10;
	PCONE &= ~(1 << 11);
	//Configuration de la led C01
	PCONC |= 1 << 2;
	PCONC &= ~(1 << 3);
	//Configuration de la led C02
	PCONC |= 1 << 4;
	PCONC &= ~(1 << 5);
	//Configuration de la led C03
	PCONC |= 1 << 6;
	PCONC &= ~(1 << 7);
}

void led(int port, int num_led, int off) {
	//On écrit une fonction générique pour les 4 LEDS :
	//grâce au premier int "port", on sélectionne le port C ou E
	//Si port == 1, on aura le port E, sinon, le port C.
	//On regarde ensuite le num_led
	//Si sa valeur est parmi les led possible (1,2 ou 3 pour C et 5 pour E)
	//Alors on va écrire sur le registre data adapté à cette position
	//Si "off == 1", la led sera éteinte, sinon, elle sera allumée
	if (port == 1){
		if (num_led == 5){
			if (off == 1)
				PDATE |= 1 << num_led;
			else
				PDATE &= ~(1 << num_led);
		}
	}
	else
		if (num_led > 0 && num_led < 4) {
			if (off == 1)
				PDATC |= 1 << num_led;
			else
				PDATC &= ~(1 << num_led);
		}
}

void all_led(int off) {
	// Cette fonction va allumer toutes les led (ou les éteindre)
	led(0,1,off);
	led(0,2,off);
	led(0,3,off);
	led(1,5,off);
}

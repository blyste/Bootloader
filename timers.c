#include "registers.h"
#include "timers.h"

void timers_init(void) {
	// On utilise le timer 0 pour la fonction sleep
	// le timer 1 pour décompter 5 sec (bootloader)
	// et le timer 3 pour le buzzer (fonction bip)
	// reset le timer 0  (par sécurité) :
	// mode one shot, inverter off, pas d'update, timer off
	TCON &= 0xfffffff0;
	// reset le timer 1 (par sécurité) :
	// mode one shot, inverter off, pas d'update, timer off
	TCON &= 0xfffff0ff;
	// reset le timer 3 du buzzer  (par sécurité) :
	// mode auto-reload, inverter on, pas d'update, timer off
	TCON &= 0xfff0ffff;
	TCON |= 0xc0000;
	
	//On a une horloge a la fréquence de 66MHz
	//On veut que le décompteur de timer 0 dure une seconde.
	//On va donc décomposer en 32*250*8250 => horloge à 1Hz
	//On règle le diviseur du timer 0 à 1/32
	TCFG1 = (TCFG1 & 0xfffffff0) | 0x4;
	//On choisit de mettre le prescalaire du timer 0 à 250
	//Remarque : c'est aussi le precalaire du timer 1 
	TCFG0 = (TCFG0 & 0xffffff00) | 250;
	// On n'utilise pas la fonction de comparaison du timer 0
	TCMPB0 = 0x0;

	//On a une horloge a la frequence de 66 MHz
	//On veut que le décompteur de timer 1 dure 5sec.
	//On va donc décomposer en 32*250*41250
	//Le prescalaire du timer 1 est déjà réglé 
	//(commun avec le timer 0)i
	//On règle le diviseur du timer 1 à 1/32
	TCFG1 = (TCFG1 & 0xffffff0f) | 0x40;
	//On n'utilise pas la fonction de comparaison du timer 1
	TCMPB1 = 0x0;
	
	//On a une horloge a la fréquence de 66MHz
	//On veut avoir une sortie qui oscille à la fréquence 2kHz.
	//On va donc décomposer en 2*165*100
	//On règle de diviseur du timer 3 à 1/2
	TCFG1 = (TCFG1 & 0xffff0fff) | 0x0000;
	//On choisit de mettre le prescalaire du timer 3 à 165
	TCFG0 = (TCFG0 & 0xffff00ff) | 165 << 8;
	// On n'utilise pas encore la fonction de comparaison du timer 3
	TCMPB3 = 0x0;
	
	//Configuration de TOUT3
	//E6 = PCONE[13,12] = 10) (branché au buzzer)
	PCONE |= 1 << 13;
	PCONE &= ~(1 << 12);
}

void sleep(void) {
	/*On veut s'endormir pendant une seconde, or l'horloge est à 66MHz
	Grâce au diviseur 1/32 et au prescalaire à 250
	On règlera le décompteur à 8250 */
	
	//Avant de commencer la configuration,
	//on s'assure que le timer 0 est arrêté
	//Si TCON[0] = 1 (marche pour le timer 0), on l'arrête.
	if (TCON & 1)
		TCON &= ~(1);
	//On précharge le timer0 à 8250
	//et en mettant le bit update manuel(TCON[1]) à 1
	TCNTB0 = 8250;
	TCON |= 1 << 1;
	//On remet le bit d'update manuel (TCON[1]) à 0
	//On met ensuite en route le décompteur (TCON[0] = 1)
	TCON &= (~(1 << 1));
	TCON |= 1;
	//The timer starts counting down after latency time
	//So we have to check that TCNTO0 != 0
	//(real beginning of counting down)
	while (TCNTO0 == 0) {}	
	//On attend ensuite que le décompteur arrive à 0
	//A ce moment la, TCNTO0 est donc à 0. 
	//1 seconde s'est alors écoulée
	while (TCNTO0 != 0) {}
}

void count_5sec(void) {
	/*Pour le bootloader : fonction load.
	 Si rien n'est transmis pendant 5 sec, on veut interrompre le flux.
	 Usage du timer 1 -> 5sec  
	 Grâce au diviseur 1/32 et au prescalaire à 250
	 On règle le décompteur à 41250 */

	//Avant de commencer la configuration
	//on s'assure qe le timer 1 est arreté
	//Si TCON[8] = 1 (marche pour le timer 1), on l'arrête.
	if (TCON & (1 << 8))
		TCON &= (~(1 << 8));
	//On précharge le timer 1 à 41250
	//et on met le bit d'update manuel (TCON[9]) à 1
	TCNTB1 = 41250;
	TCON |= 1 << 9;
	//On remet le bit d'update manuel (TCON[9]) à 0
	//On met ensuite en route le décompteur (TCON[8] = 1)
	TCON &= (~(1 << 9));
	TCON |= 1 << 8;
	//The timer starts counting down after latency time
	//So we have to check that TCNTO1 != 0
	//(real beginning of counting down)
	while (TCNTO1 == 0) {}            
}

void bip_on(void) {
	
	/*On veut générer un bip. L'horloge est à 66MHz
	 Grâce au diviseur 1/2 et au prescalaire à 165
	 On règlera le décompteur de façon à ce que 
	 la sortie TOUT3 oscille aux alentours de 2kHz*/

	//Avant de commencer la configuration,
	//on s'assure que le timer 3 est arrêté
	//Si TCON[16] = 1 (marche pour le timer 3), on l'arrête.
	if (TCON & (1 << 16))
		TCON &= (~(1 << 16));
	//On précharge le timer3 à 100
	//et en mettant le bit update manuel(TCON[17]) à 1
	//et on met le comparateur à 50
	TCNTB3 = 100;
	TCMPB0 = 50;
	TCON |= 1 << 17;

	//On remet le bit d'update manuel (TCON[17]) à 0
	//On met ensuite en route le décompteur (TCON[16] = 1)
	TCON &= (~(1 << 17));
	TCON |= 1 << 16;
	
	//Comme on est en auto-reload, à partir du lancement de la fonction
	//on a un bip jusqu'à l'appel de la fonction bip_stop.
}

void bip_off(void) {
	
	//On veut arrêter le bip
	//Le bit du décompteur (TCON[16] = 0) est donc sur STOP
	TCON &= (~(1 << 16));
}

#include "bootloader.h"
#include "registers.h"
#include "timers.h"
#include "serial.h"

unsigned char serial_getc(void){
	//Si on a reçu un octet, alors on le renvoie.
	//On va vérifier que 10 sec ne se sont pas écoulées
	//Si c'est le cas, on va brancher directement à
	//l'adresse 0x3000 en flash;
	int i = 0;
	uint32_t addr = 0x3000;
	count_5sec();
	while ((! (UTRSTAT0 & 1)) && (i != 2 )) {
		if (TCNTO1 == 0){
			i ++;
			count_5sec();
		}	
	}
	if (i == 2){
		serial_puts("Adieu\r\n");
		go(addr); 
	}
	return URXH0;

}

int serial_getc_5sec(unsigned char * a) {
	//Si on a reçu un octet avant 5 sec, 
	//on le place dans a et on renvoie 0.
	//Sinon, on renvoie 1
	//On (re)lance le timer :
	count_5sec();
	//Tant qu'on n'a pas reçu de caractères 
	//et que le timer ne vaut pas 0
	while( (!(UTRSTAT0 & 1)) && (TCNTO1 != 0));
	//Si on n'a pas reçu de caractères,
	//le timer est écoulé.
	if( ! (UTRSTAT0 & 1))
		return 1;
	*a = URXH0;
	return 0;
}


void serial_init(void){
	//Configure la vitesse du port série pour 115200 bauds à 66MHz
	UBRDIV0 = 0x23;
	//Configuration du port série sans bit de parité, avec 1 bit de stop
	//par frame et 8 bits 
	ULCON0 = 0x3;
	//Accès au port série en polling
	UCON0 = 0x5;
}

void serial_putc (const char a) {
	//Avant d'écrire un caractère, on vérifie qu'il y a
	//de la place dans le buffer pour l'écrire
	while(!(UTRSTAT0 & (1 <<1))) {}
	UTXH0 = a;
}

void serial_puts(const char * s ) {
	//Tant qu'on n'est pas en fin de chaîne,
	//on appelle serial_putc char par char
	while ( *s != 0){
		serial_putc(*s);
		s++;
	}
}

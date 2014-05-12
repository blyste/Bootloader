#include "bootloader.h"
#include "timers.h"
#include "serial.h"
#include <stdint.h>

extern int8_t _ebss;
static uint32_t fin_boot = (uint32_t) &_ebss;

//Buffer qui va stocker les commandes avant de les traiter.
static unsigned char buffer[100];

//Remplissage du buffer ; ne peut excéder 100 caractères.
//Affiche egalement dans minicom les caractères tapés.
static int fill_buffer(void) {
	for (int i = 0; i < 100; i++) {
		//On récupère le caractère
		buffer[i] = serial_getc();
		//On l'affiche dans minicom
		serial_putc(buffer[i]);
		//terminaison de la fonction avec [entrée]
		if (buffer[i] == '\r') {
			serial_putc('\n');
			buffer[i] = 0;
			return 0;
		}
	}
	return 1;
}

//Pour vider le buffer afin de pouvoir taper une nouvelle commande
static void flush_buffer(void) {
	int i = 0;
	while(buffer[i] != 0) {
		buffer[i] = 0;
		i++;
	}
}

//pour convertir un char en hexadecimal
//Si on a un caractère non hexadecimal, on decide de le remplacer par 0
static uint32_t char_to_int(const char a) {
	uint32_t i = a - 0;
	if (i > 0x29 && i < 0x3A)
		i = a - '0';
	else if (i > 0x40 && i < 0x47)
		i = a - 'A' + 10;
	else if (i > 0x60 && i < 0x67)
		i = a - 'a' + 10;
	else
		i = 0;
	return i;
}

//Pour convertir un chiffre hexadecimal en char
static char int_to_char(uint32_t i) {
	char a;
	if (i > 9)
		a = i + 'a' - 10;
	else
		a = i + '0';
	return a;
}

//Pour convertir un int32_t en string et l'imprimer dans minicom 
static void itoa(uint32_t i) {
	int x;
	for(int j = 7; j >=0; j--) {
		x = ((i>> (4*j) & 0x0f));
		serial_putc(int_to_char(x));
	}
}

//Stocke les octets arrivant sur le port en RAM à l'addresse <addr>
static int load(uint32_t addr) {
	unsigned char * p = (unsigned char*) addr;
	unsigned char a;
	//On vérifie qu'on reçoit un caractère (avant 5 secondes)
	//(dans get_byte -> serial_getc_5sec)
	while (1) {
		if(!serial_getc_5sec(&a)) {
			//On a bien reçu un octet, on l'écrit donc
			*p = a;
			//On incrémente l'adresse (pour stocker le prochain octet)
			p++; 
		}
		else {
			//On n'a pas reçu un octet en moins de 5sec
			serial_puts("Délai dépassé.\r\n");
			return 1;
		}
	}
	return 0;
}

//Transfert le contrôle à l'adresse <addr>
void go(uint32_t addr) {
	void (*p_fonction) ();
	p_fonction = (void (*) ()) addr;
	(*p_fonction)();
}

//Affiche le contenu de la mémoire à l'adresse donnée en paramètre
static void read(uint32_t addr) {
	uint32_t * p = (uint32_t*) addr;
	itoa(*p);
	//Et pour revenir à la ligne
	serial_puts("\r\n");
}

//Vérifie la validité de l'adresse : en terme de longueur 
//et en terme de valeur (adresse non utilisée par le bootloader)
static int address_checkout(uint32_t  *p) {
	//Cette fonction renvoie 0 si tout s'est bien passé.
	//On peut alors récupérer l'adresse qui est en paramètre.
	//Sinon, renvoie 1 : il faut alors afficher "adresse invalide".
	if (buffer[2] != '0' || buffer[3] != 'x'){
		serial_puts("Adresse invalide.\r\n");
		return 1;
	}
	int i = 0;
	*p = 0;
	//On récupère l'adresse tapée dans la commande (après le 'X 0x') 
	while(buffer[4 + i]) 
		*p = (*p << 4) + char_to_int(buffer[4 + i++]);
	//A la fin, on masque les 2 derniers bits par des 0
	//afin de respecter l'alignement en mémoire:
	*p &= ~(1 << 1);
	*p &= ~(1);
	//Si l'adresse fait plus de 8 caractères hexa,
	//l'adresse est invalide
	if(i > 8 || *p <= fin_boot) {
		serial_puts("Adresse invalide.\r\n");
		return 1;	
	}
	return 0;
}

static void process_buffer(void) {
	uint32_t addr;
	//Si la commande ne commence pas par L, R ou G, on le signale
	//On signale également si l'adresse n'a pas été correcte qu'a fortiori
	//l'instruction est également incorrecte
	switch (buffer[0]){
		case 'R':
			if(buffer[1] == ' '	&& (! address_checkout(&addr))) 
				read(addr);
			else
				serial_puts("Instruction incorrecte.\r\n");
			break;
		case 'L':
			if(buffer[1] == ' '	&& (! address_checkout(&addr)))
				load(addr);
			else
				serial_puts("Instruction incorrecte.\r\n");
			break;
		case 'G':
			if(buffer[1] == ' '	&& (! address_checkout(&addr)))
				go(addr);
			else 
				serial_puts("Instruction incorrecte.\r\n");
			break;
		default: 
			serial_puts("Instruction incorrecte.\r\n");
	}
}

void interface(void) {
	while (1){
		//On affiche à l'écran qu'on attend une commande par "> "
		//On attend qu'une commande soit rentrée;
		//On remplit le buffer.
		serial_puts("> ");
		if(fill_buffer()) {
			serial_puts("Commande trop longue.\r\n");
			continue;
		}
		serial_puts("Commande acquise.\r\n");
		//On traite la commande : 
		//On va vérifier seulement qu'on a une des 3 possibilités :
		// R <addr> , L <addr> ou G <addr> 
		process_buffer();
		serial_puts("Commande traitee.\r\n");
		//Une fois que la commande est traitée, on vide le buffer
		flush_buffer();
		serial_puts("Buffer flushe.\r\n");
	}
}

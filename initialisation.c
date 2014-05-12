#include <stdint.h>

extern int8_t _bss;
extern int8_t _ebss;
extern int8_t _data;
extern int8_t _edata;
extern int8_t _text;
extern int8_t _etext;
extern int8_t _rodata;
extern int8_t _erodata;

void initialisation (void) {
	// Recopie du code (.text) dans la ram (pour éviter l'execute in place)

	// On définit la LMA de .text grâce au repère du début de .rodata
	// En effet, la section .rodata est située juste après la section .text
	int8_t * LMAtext = &_rodata - &_etext + &_text ;
	int8_t * VMAtext = &_text;

	// On définit la LMA de .data grâce au repère de fin de .rodata
	// En effet, la section .rodata est située juste avant la section .data
	int8_t * LMAdata = &_erodata;
	int8_t * VMAdata = &_data;

	while (VMAtext < &_etext)
		*VMAtext++ = *LMAtext++;

	// Recopie la section .data dans la ram
	while (VMAdata < &_edata)
		*VMAdata++ = *LMAdata++;

	// Mise à zéro de la bss
	int8_t * i ;
	for ( i = &_bss ; i < &_ebss ; i++)
		*i = 0;
}

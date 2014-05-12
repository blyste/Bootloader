#ifndef REGISTERS_H
#define REGISTERS_H

#define UTRSTAT0 (*(volatile uint32_t *)0x01D00010)
#define UTXH0 (*(volatile uint8_t *)0x01D00020)
#define URXH0 (*(volatile uint8_t *)0x01D00024)
#define ULCON0 (*(volatile uint32_t *)0x01D00000)
#define UCON0 (*(volatile uint32_t *)0x01D00004)
#define UBRDIV0 (*(volatile uint32_t *)0x01D00028)

//Définition du registre de configuration des LEDS C1 à C3
#define PCONC (*(volatile uint32_t *)0x01D20010)
//Définition du registre de configuration de la lED E5
//et du buzzer TOUT 3 en E6
#define PCONE (*(volatile uint32_t *)0x01D20028) 

//Définition des registres "data" des LEDS
#define PDATC (*(volatile uint16_t *)0x01D20014)
#define PDATE (*(volatile uint16_t *)0x01D2002C)

//Définition des registres pour le timer 0, le timer 1 et le timer 3
//Définition du registre pour les prescaleurs de tous les timers
#define TCFG0 (*(volatile uint32_t *)0x01D50000)
//Définition du registre du diviseur pour tous les timers
#define TCFG1 (*(volatile uint32_t *)0x01D50004)
//Définition du registre de contrôle pour tous les timers
#define TCON (*(volatile uint32_t *)0x01D50008)
//Définition du count buffer register du timer0
#define TCNTB0 (*(volatile uint16_t *)0x01D5000C)
//Définition du compare buffer register du timer0
#define TCMPB0 (*(volatile uint16_t *)0x01D50010)
//Définition du count observation register du timer0
#define TCNTO0 (*(volatile uint16_t *)0x01D50014)
//Définition du count buffer register du timer1
#define TCNTB1 (*(volatile uint16_t *)0x01D50018)
//Définition du compare buffer register du timer1
#define TCMPB1 (*(volatile uint16_t *)0x01D5001C)
//Définition du count observation register du timer1
#define TCNTO1 (*(volatile uint16_t *)0x01D50020)
//Définition du count buffer register du timer3
#define TCNTB3 (*(volatile uint16_t *)0x01D50030)
//Définition du compare buffer register du timer3
#define TCMPB3 (*(volatile uint16_t *)0x01D50034)
//Définition du count observation register du timer3
#define TCNTO3 (*(volatile uint16_t *)0x01D50038)

#endif


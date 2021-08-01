#include <pic18f4520.h>
#include "config.h"
#include "bits.h"
#include "lcd.h"
#include "keypad.h"
#include "adc.h"
#include "pwm.h"
#include "ssd.h"
#include "stdio.h"
#include "ventilador.h"

#define L0 0x80 
#define L1 0xC0 
#define CLR 0x01
#define ON 0x0F

static int on = 0;

void start() {
    PORTD = 0x0f;
    lcdInit();
    pwmInit();
    ssdInit();
    kpInit();
    
    lcdCommand(CLR);
    lcdCommand(L0);
    int i;
    for(i = 0; i< 16; i++)
        lcdData('.');
    lcdCommand(0xC0);
    for(i = 0; i< 16; i++)
        lcdData('.');
    lcdCommand(L0);
    printf("Bem Vindo(a)   ");
    lcdCommand(L1);
    printf("1-Livre  2-Auto");
}

void func() {

    unsigned char tecla;

    kpDebounce();
    
    if (kpRead() != tecla) {
        tecla = kpRead();
        if (bitTst(tecla, 3)) {
            livreVentilador();
        }
        if (bitTst(tecla, 7)) {
            autoVentilador();
        }
    }
}

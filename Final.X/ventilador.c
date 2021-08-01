#include <pic18f4520.h>
#include "ventilador.h"
#include "pwm.h"
#include "adc.h"
#include "lcd.h"
#include "stdio.h"
#include "bits.h"
#include "keypad.h"
#include "menu.h"
#include "ssd.h"
#include "io.h"

unsigned long int tmp;
unsigned char tecla;
int tempo[4]; // h h m m
unsigned long int cont = 0;

void temporizador();
void contador(unsigned long int cont);
void display(int coluna);

void livreVentilador() { // ajusta a ventoinha com potenciometro
    adcInit();
    pwmInit();
    kpInit();
    pwmFrequency(2000);
    pwmSet1(50);
    int valor, on = 1;
    while (on) {
        lcdCommand(0x80);
        valor = adcRead(0);
        pwmSet1(valor / 10);
        lcdData(' ');
        lcdData(' ');
        lcdData(' ');
        lcdData(' ');
        lcdData(' ');
        lcdData('P');
        lcdData('o');
        lcdData('t');
        lcdData('=');
        lcdData((valor / 100) % 10 + '0');
        lcdData((valor / 10) % 10 + '0');
        lcdData((valor / 1) % 10 + '0');
        printf("\nVoltar? *-Sim");
        kpDebounce();
        if (kpRead() != tecla) {
            tecla = kpRead();
            if (bitTst(tecla, 0)) {
                pwmSet1(0);
                on = 0;
            }
        }
    }
    start();
}

void autoVentilador() { // temporiza o ventilador
    adcInit();
    pwmInit();
    
    int valor, on = 1, aux = 1;
    temporizador();
    pwmFrequency(2000);
    pwmSet1(50);

    while (on) {
        contador(cont);
        cont--;
        lcdCommand(0x80);
        valor = adcRead(0);
        pwmSet1(valor / 10);
        for (int x = 0; x < 5; x++)
            lcdData(' ');
        lcdData('P');
        lcdData('o');
        lcdData('t');
        lcdData('=');
        lcdData((valor / 100) % 10 + '0');
        lcdData((valor / 10) % 10 + '0');
        lcdData((valor / 1) % 10 + '0');
        for (int x = 0; x < 4; x++)
            lcdData(' ');
        printf("\n Voltar? Pot=0");
        
        if (cont == 0)
            on = 0;
        if (valor < 10)
            on = 0;
    }
    pwmSet1(0);
    start();
}

void display(int coluna) {
    switch (coluna) {
        case 0: lcdCommand(0xC5);
            break;
        case 1: lcdCommand(0xC6);
            break;
        case 2: lcdCommand(0xC8);
            break;
        case 3: lcdCommand(0xC9);
            break;
    }
}

void contador(unsigned long int cont) {
    tmp--;
    ssdDigit(((cont / 1) % 10), 3); // ss
    ssdDigit(((cont / 10) % 6), 2); // ss
    ssdDigit(((cont / 60) % 10), 1); // mm
    ssdDigit(((cont / 600) % 10), 0); // mm
    int i, j;
    for (i = 0; i < 120; i++) {
        ssdUpdate();
        for (j = 0; j < 800; j++);
    }
}

void temporizador() {
    ssdInit();
    kpInit();
    
    tecla = kpRead();
    int aux = 4;
    int i = 0;
    lcdCommand(0x0F);
    lcdCommand(0x80);
    printf("Tempo Desejado: ");
    printf("\n.... MM:SS ....");
    display(0);
    while (aux) {
        kpDebounce();
        if (kpRead() != tecla) {   
            tecla = kpRead();
            display(i);
            if (bitTst(tecla, 3)) {
                tempo[i] = 1;
                lcdData('1');
                i++;
                aux--;
            }
            if (bitTst(tecla, 7)) {
                tempo[i] = 2;
                lcdData('2');
                i++;
                aux--;
            }
            if (bitTst(tecla, 10)) {
                tempo[i] = 3;
                lcdData('3');
                i++;
                aux--;
            }
            if (bitTst(tecla, 2)) {
                tempo[i] = 4;
                lcdData('4');
                i++;
                aux--;
            }
            if (bitTst(tecla, 6)) {
                tempo[i] = 5;
                lcdData('5');
                i++;
                aux--;
            }
            if (bitTst(tecla, 8)) {
                tempo[i] = 6;
                lcdData('6');
                i++;
                aux--;
            }
            if (bitTst(tecla, 1)) {
                tempo[i] = 7;
                lcdData('7');
                i++;
                aux--;
            }
            if (bitTst(tecla, 5)) {
                tempo[i] = 8;
                lcdData('8');
                i++;
                aux--;
            }
            if (bitTst(tecla, 9)) {
                tempo[i] = 9;
                lcdData('9');
                i++;
                aux--;
            }
            if (bitTst(tecla, 4)) {
                tempo[i] = 0;
                lcdData('0');
                i++;
                aux--;
            }
            if (bitTst(tecla, 0)) {
                if (i != 0) {
                    i--;
                    tempo[i] = 99;
                    aux++;
                }
            }
        }
    }
    while (tempo[2] > 5) {
        tempo[1]++;
        tempo[2]--;
    }
    while (tempo[1] > 9) {
        tempo[1]--;
        tempo[0]++;
    }
    if (tempo[0] <= 9) {
        cont = (tempo[0]*600) + (tempo[1]*60) + (tempo[2]*10) + (tempo[3]*1);
    } else {
        tempo[0] = 9;
        cont = tempo[0]*600 + tempo[1]*60 + tempo[2]*10 + tempo[3]*1;
    }
    tmp = cont + 12;

}

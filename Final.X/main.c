#include <pic18f4520.h> //projeto final
#include "config.h"
#include "bits.h"
#include "lcd.h"
#include "keypad.h"
#include "adc.h"
#include "pwm.h"
#include "ssd.h"
#include "stdio.h"
#include "ventilador.h"
#include "menu.h"

void main(void) {

    int tmp = 0;
    
    start();
    
    while (1) {
        func();
    }
}

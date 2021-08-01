#include "stdio.h"
#include "lcd.h"

void printf(char c[16]) {
    int i;
    for (i = 0; i < 16; i++) {
        if(c[i] == '\n')
            lcdCommand(0xC0);
        else
            lcdData(c[i]);
    }
}

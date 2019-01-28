#include "headers/led.h"
#include "headers/lcd.h"
#include "headers/utils.h"
#include "headers/swt.h"

#include <stdio.h>
#include <string.h>
#include <xc-pic32m.h>
#include "stdbool.h"

void switchLed(void);


int main(void){
    LED_Init();
    LCD_Init();
    SWT_Init();
//    __XC_UART = 1;
    printf("%s\n", "tere vana kere");
    
    LED_SetValue(7,1);
    LCD_WriteStringAtPos("Hello World!", 0, 0);

    switchLed();
    return 0;
}


void switchLed(void){
    printf("%s", "tere");
    //printf("See on: %u\n", SWT_GetGroupValue());
}
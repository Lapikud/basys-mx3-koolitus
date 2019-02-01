#include "headers/led.h"
#include "headers/lcd.h"
#include "headers/utils.h"
#include "headers/swt.h"

#include <stdio.h>
#include <string.h>
#include <xc-pic32m.h>
#include "stdbool.h"

// Prototypes

//Checks value of swt and changes led either on or off
void switchOneLed(unsigned char value);
void switchAllLeds(void);


int main(void){
	// init LED, LCD and switches
    LED_Init();
    LCD_Init();
    SWT_Init();

    switchLed(1);
    switchAllLeds();
    return 0;
}


//Checks value of swt and changes led either on or off
void switchOneLed(unsigned char value){
    if (SWT_GetValue(value) {
		LED_SetValue(value, 1);
	} else {
		LED_SetValue(value, 0);
	}
}

//Checks all swt values and changes leds.
void switchAllLeds(void){
	int i = 0;
	for(i = 0; i<8; i++){
		switchOneLed(i);
	}
}

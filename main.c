#include "headers/led.h"
#include "headers/lcd.h"
#include "headers/utils.h"
#include "headers/swt.h"
#include "headers/btn.h"
#include "headers/audio.h"
#include "headers/rgbled.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <xc-pic32m.h>
#include "stdbool.h"

// Prototypes

//Checks value of swt and changes led either on or off
void switchLed(unsigned char value);
void init(void);

int pinCode[4];

int main(void){
	// init LED, LCD and switches
    int i; 
    char number;
    int c = 0;
    init();
    pinCode[1] = 0;
    pinCode[2] = 3;
    pinCode[3] = 4;
    pinCode[4] = 5; 
   
    LCD_WriteStringAtPos("Sisesta PIN kood", 0, 0);
    while(true){
        
        for(i = 0; i < 8; i++){
            
            if(SWT_GetValue(i)){
                LED_SetValue(7, 1);
                c = 1;
                break;
            }
        }
        if(c){
            break;
        }
    
    }
    LCD_DisplayClear();
    //itoa(i, number, 10);
    
    LCD_WriteStringAtPos("2", 0, 0);
   
    
    /*while(true){
    LED_SetValue(1,BTN_GetValue('C'));
    DelayAprox10Us(1);
    }*/
        
    
    return 0;
}


void init(void){
    LED_Init();
    LCD_Init();
    SWT_Init();
    BTN_Init();
    //AUDIO_Init(0);

}
//Checks value of swt and changes led either on or off
void switchLed(unsigned char value){
    if (SWT_GetValue(value)) {
		LED_SetValue(value, 1);
	}
}

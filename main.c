#include "headers/led.h"
#include "headers/lcd.h"
#include "headers/utils.h"
#include "headers/swt.h"
#include "headers/ssd.h"
#include "headers/audio.h"

#include <stdio.h>
#include <string.h>
#include <xc-pic32m.h>
#include <stdbool.h>

// Prototypes

//Checks value of swt and changes led either on or off
void switchOneLed(unsigned char value);
void switchAllLeds(void);
void displaySegment(int enteredPin[]);
int checkPin(int sentPin[]);
void displaySegment(int enteredPin[]);
int checkSafe();
void toggleRGB();
void sesamOpen();
int swtChanged();
int switchSelected();

int pin[4] = {0, 1, 2, 3};
int resetPin[4] = {0, 0, 0, 0};

int main(void){
	// init LED, LCD and switches
    LED_Init();
    LCD_Init();
    SWT_Init();
	SSD_Init();
	if (checkSafe() == 1) {
		sesamOpen();
		toggleRGB();
	}

    return 0;
}

void sesamOpen()
{
    LCD_WriteStringAtPos("PIN kood on OK!", 0, 0);
    AUDIO_Init(1);
    
}

int switchSelected()
{ // output : swts
    int i;
    
    for (i = 0; i < 8 ; i++){
        if(SWT_GetValue(i)) return i;
    }
}

int swtChanged()
{
    if (SWT_GetGroupValue() > 0){
        return 1;
    }else{
        return 0;
    }
}

int checkPin(int sentPin[]){
	// returns 0 if wrong pin
	// return 1 on correct pin
	int i;
	for(i = 0; i < 4; i++){
		if(sentPin[i] != pin[i]){
			return 0;
		}
	}
	return 1;

}

void displaySegment(int enteredPin[]){
	SSD_WriteDigits(enteredPin[3], enteredPin[2], enteredPin[1], enteredPin[0],
	0, 0, 0, 0);
}

int checkSafe(){
	// result 0 = false
	// result 1 = true 
	int result = 0;
	int enteredPin[4] = {0, 0, 0, 0};
	
	//counters
	int i, pinCounter;
	// check if pin is correct
    bool input = false;
	while(true){
		//init-reset enteredPin to 0,0,0,0
		enteredPin[0] = 1;
		enteredPin[1] = 2;
		enteredPin[2] = 3;
		enteredPin[3] = 4;
		pinCounter = 0;
		while(true){
			//listen to swt inputs
            if (pinCounter == 0) input = false;
            
			displaySegment(enteredPin);
			if(swtChanged()){
                if(!input){
                    switchOneLed(7);				
                    input = true;
                    enteredPin[pinCounter] = switchSelected();				
                    pinCounter++;
                }  
            } else {
                input = false;
            }
			if (pinCounter >= 5){break;}
            DelayAprox10Us(1000);
		}
		



		if (checkPin(enteredPin) == 1) {
			LCD_WriteStringAtPos("korras", 0, 0);
            result = 1;
			break;
		}
	}
	
	
	
	return result;
}

//Checks value of swt and changes led either on or off
void switchOneLed(unsigned char value){
    if (SWT_GetValue(value)) {
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

void toggleRGB()
{
    int i;
    for (i = 0; i < 3 ; i++){
        switch (i){
            case 0:
                RGBLED_SetValue(255,0, 0);
                break;
            case 1:
                RGBLED_SetValue(0,255,0);
                break;
            case 2:
                RGBLED_SetValue(0,0, 255);
                i = -1;
        }
        DelayAprox10Us(1000);
    }

}
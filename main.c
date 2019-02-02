#include "headers/led.h"
#include "headers/lcd.h"
#include "headers/utils.h"
#include "headers/swt.h"
#include "headers/ssd.h"

#include <stdio.h>
#include <string.h>
#include <xc-pic32m.h>
#include "stdbool.h"

// Prototypes

//Checks value of swt and changes led either on or off
void switchOneLed(unsigned char value);
void switchAllLeds(void);
void displaySegment(int enteredPin[]);

int pin[4] = {0, 1, 2, 3};
const int resetPin[4] = {0, 0, 0, 0};

int main(void){
	// init LED, LCD and switches
    LED_Init();
    LCD_Init();
    SWT_Init();
	SSD_Init();


    switchLed(1);
    switchAllLeds();
    return 0;
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

int checkSafe(int pin[]){
	// result 0 = false
	// result 1 = true 
	int result = 1;
	int enteredPin[4];
	
	//counters
	int i, pinCounter;
	// check if pin is correct
	while(true){
		//init-reset enteredPin to 0,0,0,0
		enteredPin = resetPin;
		pinCounter = 0;
		while(true){
			//listen to swt inputs
			bool input = false;
			displaySegment(enteredPin);
			if(swtChanged() == 1){
				input = true;
				enteredPin[pinCounter] == switchSelected();				
				pinCounter++;
			} else if(swtChanged() == 0) {
				input = false;
			}
			if (pinCounter == 3){break;}
		}
		



		if (checkPin(enteredPin) == 1) {
			LCD_WriteStringAtPos("korras", 0, 0);
			break;
		}
	}
	
	
	
	return result;
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

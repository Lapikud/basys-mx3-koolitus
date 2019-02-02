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
#pragma config JTAGEN = OFF     
#pragma config FWDTEN = OFF      


/* ------------------------------------------------------------ */
/*						Configuration Bits		                */
/* ------------------------------------------------------------ */


// Device Config Bits in  DEVCFG1:	
#pragma config FNOSC =	FRCPLL
#pragma config FSOSCEN =	OFF
#pragma config POSCMOD =	XT
#pragma config OSCIOFNC =	ON
#pragma config FPBDIV =     DIV_2
// Device Config Bits in  DEVCFG2:	
#pragma config FPLLIDIV =	DIV_2
#pragma config FPLLMUL =	MUL_20
#pragma config FPLLODIV =	DIV_1
// Prototypes

//Checks value of swt and changes led either on or off
void init(void);
void sesamOpen();
void toggleRGB();
int switchSelected();
int checkPin(int sentPin[], int pin[]);
void displaySegment(int enteredPin[]);
int swtChanged();
int checkSafe(int pin[]);


int main(void){
    int pin[4] = {0, 1, 2, 3};
    
    init();

    if(checkSafe(pin)){
        sesamOpen();
        toggleRGB();
    }
    
    return 0;
}


void init(void){
    LED_Init();
    LCD_Init();
    SWT_Init();
    BTN_Init();
    RGBLED_Init();
    SSD_Init();

}

void sesamOpen()
{
    LCD_WriteStringAtPos("PIN kood on OK!", 0, 0);
    
}

void toggleRGB()
{
    int i;
    for (i = 0; i < 3 ; i++){
        switch (i){
            case 0:
                RGBLED_SetValue(255,0, 0);//RED
                break;
            case 1:
                RGBLED_SetValue(0,255,0);//GREEN
                break;
            case 2:
                RGBLED_SetValue(0,0, 255);//BLUE
                i = -1;
        }
        DelayAprox10Us(1000);
    }

}
int switchSelected(){ 
    // output : turne on switch
    int i;
    
    for (i = 0; i < 8 ; i++){
        if(SWT_GetValue(i)) return i;
    }
}
int checkPin(int sentPin[], int pin[]){
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
int swtChanged()
{
    if (SWT_GetGroupValue() > 0){
        return 1;
    }else{
        return 0;
    }
}
int checkSafe(int pin[]){
	// result 0 = false
	// result 1 = true 
	int result = 0;
	int enteredPin[4];
	
	//counters
	int i, pinCounter;
	// check if pin is correct
    bool input = false;
	while(true){
        LCD_DisplayClear();
        LCD_WriteStringAtPos("Sisesta PIN kood", 0, 0);
		//init-reset enteredPin to 0,0,0,0
		enteredPin[0] = 0;
		enteredPin[1] = 0;
		enteredPin[2] = 0;
		enteredPin[3] = 0;
		pinCounter = 0;
		while(true){
			//listen to swt inputs
            
			displaySegment(enteredPin);
			if(swtChanged()){
                if(!input){				
                    input = true;
                    enteredPin[pinCounter] = switchSelected();				
                    pinCounter++;
                }  
            } else {
                input = false;
            }
			if (pinCounter == 4){displaySegment(enteredPin);break;}
            DelayAprox10Us(1000);
		}

		if (checkPin(enteredPin,pin) == 1) {
			LCD_DisplayClear();
            result = 1;
			break;
		} else {
            LCD_DisplayClear();
            LCD_WriteStringAtPos("Vale PIN kood!", 0, 0);
            DelayAprox10Us(5000);
        }
	}
	
	return result;
}


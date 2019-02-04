#include "headers/led.h"
#include "headers/lcd.h"
#include "headers/utils.h"
#include "headers/swt.h"
#include "headers/btn.h"
#include "headers/audio.h"
#include "headers/rgbled.h"
#include "headers/ssd.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <xc-pic32m.h>
#include <stdbool.h>
#pragma config JTAGEN = OFF
#pragma config FWDTEN = OFF

/* ------------------------------------------------------------ */
/*						Configuration Bits		                */
/* ------------------------------------------------------------ */

// Device Config Bits in  DEVCFG1:
#pragma config FNOSC = FRCPLL
#pragma config FSOSCEN = OFF
#pragma config POSCMOD = XT
#pragma config OSCIOFNC = ON
#pragma config FPBDIV = DIV_2
// Device Config Bits in  DEVCFG2:
#pragma config FPLLIDIV = DIV_2
#pragma config FPLLMUL = MUL_20
#pragma config FPLLODIV = DIV_1

// Prototypes

void init(void);
void sesameOpen();
void toggleRGB();
int switchSelected();
int checkPin(int sentPin[], int pin[]);
void displaySegment(int enteredPin[]);
int swtChanged();
int checkSafe(int pin[]);
void LEDTogglBySwitch();
int BTNPressed(unsigned char btnLetter);

int main(void)
{
	int pin[4] = {0, 1, 2, 3};

	init();

	if (checkSafe(pin))
	{
		sesameOpen();
		toggleRGB();
	}

	return 0;
}

/* ------------------------------------------------------------ */
/***	init
**
**	Parameters:
**
**
**	Return Value:
**
**
**	Description:
**		Initialize LED, LCD, SWT, BTN, RGBLED and SSD(Seven segment display) modules.
**		This is needed to prepare hardware for modules.
**
*/
void init(void)
{
	LED_Init();
	LCD_Init();
	SWT_Init();
	BTN_Init();
	RGBLED_Init();
	SSD_Init();
}

/* ------------------------------------------------------------ */
/***	sesameOpen
**
**	Parameters:
**
**
**	Return Value:
**
**
**	Description:
**		Displays confirmation message on success.
**
*/
void sesameOpen()
{
	LCD_WriteStringAtPos("PIN kood on OK!", 0, 0);
}

/* ------------------------------------------------------------ */
/***	toggleRGB
**
**	Parameters:
**
**
**	Return Value:
**
**
**	Description:
**		Flash red, green and blue RGBLed
**		NB! This function does not exit!
**		Remember to use this at the end if your code!
**
*/
void toggleRGB()
{
	int i;
	for (i = 0; i < 3; i++)
	{
		switch (i)
		{
		case 0:
			RGBLED_SetValue(255, 0, 0); //RED
			break;
		case 1:
			RGBLED_SetValue(0, 255, 0); //GREEN
			break;
		case 2:
			RGBLED_SetValue(0, 0, 255); //BLUE
			i = -1;
		}
		DelayAprox10Us(1000);
	}
}

/* ------------------------------------------------------------ */
/***	switchSelected
**
**	Parameters:
**
**
**	Return Value:
**		int i			- number of the switch that was enable between 0 - 7
**
**
**	Description:
**		Only returns value if there is a switch turned on,
**		in case of multiple switches on, returns first found
**
*/
int switchSelected()
{
	int i;

	for (i = 0; i < 8; i++)
	{
		if (SWT_GetValue(i)) return i;
	}
}

/* ------------------------------------------------------------ */
/***	checkPin
**
**	Parameters:
**		int sentPin		- Pin that the user entered
**		int pin			- Correct pin
**
**	Return Value:
**		int			- Value of true and false if sentPin matched
**							0 when pin was wrong
**							1 when pin was correct
**
**	Description:
**		Compares 4 digit pin with another 4 digit pin
**      Returns 1 or 0 based on result
**
*/
int checkPin(int sentPin[], int pin[])
{
	int i;
	for (i = 0; i < 4; i++)
	{
		if (sentPin[i] != pin[i])
		{
			return 0;
		}
	}
	return 1;
}

/* ------------------------------------------------------------ */
/***	displaySegment
**
**	Parameters:
**		int enteredPin[]- pin from user input
**
**	Return Value:
**
**
**	Description:
**		Map entered pin to SSD (Seven Segment Display)
**		Its mapped from right to left
**
*/
void displaySegment(int enteredPin[])
{
	SSD_WriteDigits(enteredPin[3], enteredPin[2], enteredPin[1], enteredPin[0],
					0, 0, 0, 0);
}

/* ------------------------------------------------------------ */
/***	swtChanged
**
**	Parameters:
**
**	Return Value:
**		int 		- Boolean if any swt is active
**							0 when none active
**							1 when any is active
**
**	Description:
**		Shows if any of the switches is active
**
*/
int swtChanged()
{
	if (SWT_GetGroupValue() > 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/* ------------------------------------------------------------ */
/***	checkSafe
**
**	Parameters:
**		int pin[]	- Value of user set pin as an array
**
**	Return Value:
**		int result	- Shows if safe is opened
**							1 when safe is opened
**
**	Description:
**		Lets user input values from switches and displays instructions
**		on LCD for what to do. Only exits (with 1) when pins match.
**
*/
int checkSafe(int pin[])
{
	// result 0 = false
	// result 1 = true
	int result = 0;
	int enteredPin[4];

	//counters
	int i, pinCounter;
	// check if pin is correct
	bool input = false;
	while (true)
	{
		LCD_DisplayClear();
		LCD_WriteStringAtPos("Sisesta PIN kood", 0, 0);
		//init-reset enteredPin to 0,0,0,0
		enteredPin[0] = 0;
		enteredPin[1] = 0;
		enteredPin[2] = 0;
		enteredPin[3] = 0;
		pinCounter = 0;
		while (true)
		{
			//listen to swt inputs

			displaySegment(enteredPin);
			LEDTogglBySwitch();
			if (swtChanged())
			{
				if (!input)
				{
					input = true;
					enteredPin[pinCounter] = switchSelected();
					pinCounter++;
				}
			}
			else
			{
				input = false;
			}
			if (pinCounter == 4)
			{
				displaySegment(enteredPin);
				break;
			}
			DelayAprox10Us(1000);
		}

		if (checkPin(enteredPin, pin) == 1)
		{
			LCD_DisplayClear();
			result = 1;
			break;
		}
		else
		{
			LCD_DisplayClear();
			LCD_WriteStringAtPos("Vale PIN kood!", 0, 0);
			DelayAprox10Us(5000);
		}
	}

	return result;
}

/* ------------------------------------------------------------ */
/***	LEDTogglBySwitch
**
**	Parameters:
**
**	Return Value:
**
**
**	Description:
**		Flips LED on or off based on SWT state.
**		Example: SWT 0 is on, so LED 0 is on
**
*/
void LEDTogglBySwitch()
{
	int i;
	for (i = 0; i < 8; i++)
	{
		LED_SetValue(i, SWT_GetValue(i));
	}
}

/* ------------------------------------------------------------ */
/***	BTNPressed
**
**	Parameters:
**      btnLetter - letter of button on board
**                  'R' , 'L' , 'U', 'D', 'C'
**
**	Return Value:
**         1 - if chosen button is pressed
**
**	Description:
**		When chosen button is pressed down, a function returns 1,
**		otherwise function is running
**
*/
int BTNPressed(unsigned char btnLetter)
{
    while(true)
    {
        if (BTN_GetValue(btnLetter)) // true, if button is pressed
        {
            return 1;
        }
    }
}


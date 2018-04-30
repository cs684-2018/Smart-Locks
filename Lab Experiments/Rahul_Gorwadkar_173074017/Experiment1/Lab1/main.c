/*

* Author:

* Reference Code By: Saurav Shandilya, Vishwanathan Iyer
* 					ERTS Lab, CSE Department, IIT Bombay

* Editted by: Rahul Gorwadkar
	      ERTS Lab, CSE Department, IIT Bombay

* Description: This code will blink three colors of RGB LED present of Launchpad board individually in sequence only when key is pressed.

* Filename: lab-1-1.c

* Functions: setup(), led_pin_config(), main(), sw_pin_config()

* Global Variables: ui8LED, key_detect, key_detect_release;

*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

/*

* Function Name: setup()

* Input: none

* Output: none

* Description: Set crystal frequency and enable GPIO Peripherals

* Example Call: setup();

*/
void setup(void)
{
	SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
}

/*

* Function Name: led_pin_config()

* Input: none

* Output: none

* Description: Set PORTF Pin 1, Pin 2, Pin 3 as output. On this pin Red, Blue and Green LEDs are connected.

* Example Call: led_pin_config();

*/

void led_pin_config(void)
{
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
}

/*

* Function Name: sw_pin_config()

* Input: none

* Output: none

* Description: Set PORTF Pin 4 as input. On this pin Switch 1 is connected.

* Example Call: sw_pin_config();

*/
void sw_pin_config(void)
{
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);

	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}


int main(void)
{
	uint8_t ui8LED = 2;
	//key_detect_release is flag to ensure key is released before detecting key press
	uint32_t key_detect, key_detect_release = 1;

	setup();
	led_pin_config();
	sw_pin_config();

	while(1)
	{
		key_detect = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);
		if(((key_detect & 0x00000010) == 0) && (key_detect_release == 1))
		{
			//Debounce Delay approx 37ms
			SysCtlDelay(500000);
			if(((key_detect & 0x00000010) == 0))
			{
				key_detect_release = 0;
				// Cycle through Red, Green and Blue LEDs
				if (ui8LED == 8)
				{
					ui8LED = 2;
				}
				else
				{
					ui8LED = ui8LED*2;
				}
				// Turn on the LED
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8LED); //0x0E);
			}
		}

		if(((key_detect & 0x00000010) == 0x00000010) && (key_detect_release == 0))
		{
			//Debounce Delay approx 37ms
			SysCtlDelay(500000);
			if((key_detect & 0x00000010) == 0x00000010)
			{
				key_detect_release = 1;
				//Turn off LEDs when key is released
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
			}
		}
	}
}

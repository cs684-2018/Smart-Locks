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
#include "inc/tm4c123gh6pm.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"

#define IDLE 1
#define PRESS 2
#define RELEASE 3

uint8_t debounce_flag=0;
uint8_t switch_status=IDLE;
uint8_t ui8LED = 2;

/*

* Function Name: setup()

* Input: none

* Output: none

* Description: Set crystal frequency and enable GPIO Peripherals

* Example Call: setup();

*/
void setup(void)
{
	//SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	//SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN); //To Run at 40Mhz
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
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
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01 ;
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;

	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);

	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}



int main(void)
{
	uint32_t ui32Period;

	setup();
	led_pin_config();
	sw_pin_config();

	ui32Period = (SysCtlClockGet() / 100) / 2;
	TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);

	IntEnable(INT_TIMER0A);
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	IntMasterEnable();
	TimerEnable(TIMER0_BASE, TIMER_A);

	while(1)
	{

	}

}

void Timer0IntHandler(void)
{
	uint32_t key_detect;
	// Clear the timer interrupt
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

	key_detect = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);

	if((key_detect & 0x00000010) == 0)
	{
		switch(switch_status)
		{
			case IDLE:
				if(debounce_flag == 0)
					debounce_flag = 1;
				else
				{
					// Cycle through Red, Green and Blue LEDs
					if (ui8LED == 8)
					{
						ui8LED = 2;
					}
					else
					{
						ui8LED = ui8LED*2;
					}
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8LED);
					switch_status = PRESS;
					debounce_flag = 0;
				}
				break;
			case PRESS:
				switch_status = RELEASE;
				debounce_flag = 0;
				break;
			case RELEASE:
				switch_status = RELEASE;
				debounce_flag = 0;
				break;
			default:
				switch_status = IDLE;
				debounce_flag = 0;
				break;
		}
	}
	else
	{
		switch(switch_status)
		{
			case IDLE:
				//debounce_flag = 0;
				break;
			case PRESS:
				if(debounce_flag == 0)
					debounce_flag = 1;
				else
				{
					switch_status = IDLE;
					debounce_flag = 0;
				}
				break;
			case RELEASE:
				if(debounce_flag == 0)
					debounce_flag = 1;
				else
				{
					switch_status = IDLE;
					debounce_flag = 0;
				}
				break;
			default:
				switch_status = IDLE;
				debounce_flag = 0;
				break;
		}
	}
}

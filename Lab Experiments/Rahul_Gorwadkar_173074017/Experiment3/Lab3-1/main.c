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
#include "driverlib/rom.h"
#include "driverlib/pin_map.h"
#include "driverlib/debug.h"
#include "driverlib/pwm.h"

#define PWM_FREQUENCY 55

#define RB 0
#define BY 1
#define YR 2

#define RED 2
#define BLUE 3
#define GREEN 4

#define IDLE 1
#define PRESS 2
#define RELEASE 3

uint8_t debounce_flag1=0;
uint8_t debounce_flag2=0;
uint8_t switch_status1=IDLE;
uint8_t switch_status2=IDLE;
unsigned char counter=1,TempCounter =1;
volatile uint32_t ui32Load;
uint8_t adjust_status=IDLE;
uint32_t adjust_count_long=0;
uint32_t adjust_count_short=0;

void timer0init(void)

{
	uint32_t ui32Period;

	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
	ui32Period = (SysCtlClockGet() / 40) / 2;
	TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);
	IntEnable(INT_TIMER0A);
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	IntMasterEnable();
	TimerEnable(TIMER0_BASE, TIMER_A);
}

int main(void)
{

	unsigned int RedVal=255,BlueVal=0,GreenVal =0,state =  0;

	//volatile uint32_t ui32Load;
	volatile uint32_t ui32PWMClock;
	volatile uint8_t ui8Adjust;

	ui8Adjust = 254;

	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
	SysCtlPWMClockSet(SYSCTL_PWMDIV_64);

	timer0init();

	//Init Peripherals
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

	//Unlock SW2 or PortF.0
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;

	//Set IO Proerties
	GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_DIR_MODE_IN);
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

	//Calculating PWM Clock
	ui32PWMClock = SysCtlClockGet() / 64;
	ui32Load = (ui32PWMClock / (PWM_FREQUENCY)) - 1;


	/************GPIO F-1configuration ****************/
	/*******Start*********/
	GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1);
	GPIOPinConfigure(GPIO_PF1_M1PWM5);

	PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN);
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, ui32Load);
	PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, true);
	PWMGenEnable(PWM1_BASE, PWM_GEN_2);
	/******End*********/

	/************GPIO F-2configuration ****************/
	/*******Start*********/
	GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2);
	GPIOPinConfigure(GPIO_PF2_M1PWM6);
	PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN);
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, ui32Load);
	PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, true);
	PWMGenEnable(PWM1_BASE, PWM_GEN_3);
	/******End*********/

	/************GPIO F-3configuration ****************/
	/*******Start*********/
	GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_3);
	GPIOPinConfigure(GPIO_PF3_M1PWM7);
	PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN);
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, ui32Load);
	PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, true);
	PWMGenEnable(PWM1_BASE, PWM_GEN_3);
	/******End*********/

	while(1)
	{
		switch(switch_state)
		{
			case IDLE:
				PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, GreenVal * ui32Load / 1000);
				PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, BlueVal * ui32Load / 1000);
				PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, RedVal * ui32Load / 1000);
				break;

			case RED:
				PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, 0 * ui32Load / 1000);
				PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, 0 * ui32Load / 1000);
				PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, RedVal * ui32Load / 1000);
				break;

			case BLUE:
				PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, 0 * ui32Load / 1000);
				PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, BlueVal * ui32Load / 1000);
				PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, 0 * ui32Load / 1000);
				break;

			case GREEN:
				PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, GreenVal * ui32Load / 1000);
				PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, 0 * ui32Load / 1000);
				PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, 0 * ui32Load / 1000);
				break;
		}

		if((switch_status1 == RELEASE) && (switch_status2 == RELEASE))
		{
			adjust_status = BLUE;
		}
		if((adjust_count_long == 300) && (switch_status2 == RELEASE)) //&& (switch_status2 == RELEASE)
		{
			if(adjust_count_short == 1)
			{
				adjust_status = RED;
			}
			if(adjust_count_short == 2)
			{
				adjust_status = GREEN;
			}
		}
		if((adjust_count_long != 300))
		{
			//if(adjust_status == BLUE)
		}

			//if((switch_status1 == IDLE) && (switch_status2 == IDLE))
			//{
				//PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, GreenVal * ui32Load / 1000);
				//PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, BlueVal * ui32Load / 1000);
				//PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, RedVal * ui32Load / 1000);
				//PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, 2 * ui32Load / 1000);
				//PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, 2 * ui32Load / 1000);
				//PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, 2 * ui32Load / 1000);
			//}
	}
}

void Timer0IntHandler(void)
{
	uint32_t key_detect;

	// Clear the timer interrupt
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

	TempCounter++;
	counter++;
	if(counter>254)
		counter =1;

	key_detect = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0);

	if((key_detect & 0x00000001) == 0)
	{
		switch(switch_status1)
		{
			case IDLE:
				if(debounce_flag1 == 0)
				{
					debounce_flag1 = 1;
					//adjust_count = 0;
				}
				else
				{
					//sw2status++;
					switch_status1 = PRESS;
					debounce_flag1 = 0;
				}
				break;
			case PRESS:
				switch_status1 = RELEASE;
				debounce_flag1 = 0;
				break;
			case RELEASE:
				switch_status1 = RELEASE;
				debounce_flag1 = 0;
				adjust_count_long++;
				if(adjust_count_long > 300)
					adjust_count_long = 300;
				//PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, 20 * ui32Load / 1000);
				//PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, 20 * ui32Load / 1000);
				//PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, 20 * ui32Load / 1000);
				//GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x0E);
				break;
			default:
				switch_status1 = IDLE;
				debounce_flag1 = 0;
				break;
			}
	}
	else
	{
		switch(switch_status1)
		{
			case IDLE:
				//debounce_flag = 0;
				break;
			case PRESS:
				if(debounce_flag1 == 0)
				{
					debounce_flag1 = 1;
					//adjust_count_long = 0;
				}
				else
				{
					switch_status1 = IDLE;
					debounce_flag1 = 0;
					adjust_count_long = 0;
				}
				break;
			case RELEASE:
				if(debounce_flag1 == 0)
					debounce_flag1 = 1;
				else
				{
					switch_status1 = IDLE;
					debounce_flag1 = 0;
				}
				break;
			default:
				switch_status1 = IDLE;
				debounce_flag1 = 0;
				break;
		}
	}

	key_detect = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);
	if((key_detect & 0x00000010) == 0)
	{
		switch(switch_status2)
		{
			case IDLE:
				if(debounce_flag2 == 0)
					debounce_flag2 = 1;
				else
				{
					//sw2status++;
					switch_status2 = PRESS;
					debounce_flag2 = 0;
				}
				break;
			case PRESS:
				switch_status2 = RELEASE;
				debounce_flag2 = 0;
				adjust_count_short++;
				break;
			case RELEASE:
				switch_status2 = RELEASE;
				debounce_flag2 = 0;
				//if(adjust_count_long == 300)
				//{
					//adjust_count_short++;
					//if(adjust_count_short == 1)
					//	adjust_state = RED;
					//if(adjust_count_short == 2)
					//	adjust_state = GREEN;
				//}
				//PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, 80 * ui32Load / 1000);
				//PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, 80 * ui32Load / 1000);
				//PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, 80 * ui32Load / 1000);
				break;
			default:
				switch_status2 = IDLE;
				debounce_flag2 = 0;
				//GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x0E);
				break;
		}
	}
	else
	{
		switch(switch_status2)
		{
			case IDLE:
				//debounce_flag = 0;
				break;
			case PRESS:
				if(debounce_flag2 == 0)
					debounce_flag2 = 1;
				else
				{
					switch_status2 = IDLE;
					debounce_flag2 = 0;
				}
				break;
			case RELEASE:
				if(debounce_flag2 == 0)
					debounce_flag2 = 1;
				else
				{
					switch_status2 = IDLE;
					debounce_flag2 = 0;
				}
				break;
			default:
				switch_status2 = IDLE;
				debounce_flag2 = 0;
				break;
		}
	}
}


/*
		switch(state)
		{
			case RB :
				//counter=1;
				RedVal = 255-counter;
				BlueVal =counter;
				GreenVal =1;
				if(BlueVal>250)
				{
					counter =1;
					state  = BY;
				}
				break;

			case BY :
				BlueVal= 255-counter;
				GreenVal =counter;
				RedVal =1;
				if(GreenVal>250)
				{
					state  = YR;
					counter =1;
				}
				break;

			case YR :
				GreenVal= 255-counter;
				RedVal =counter;
				BlueVal =1;
				if(RedVal>250)
				{
					state  = RB;
					counter =1;
				}
				break;
		}

		if(TempCounter >2)
		{
			TempCounter =0;
			if((switch_status1 == IDLE) && (switch_status2 == IDLE))
			{
				//PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, GreenVal * ui32Load / 1000);
				//PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, BlueVal * ui32Load / 1000);
				//PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, RedVal * ui32Load / 1000);
				PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, 2 * ui32Load / 1000);
				PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, 2 * ui32Load / 1000);
				PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, 2 * ui32Load / 1000);
			}
		}

		if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)==0x00)
		{
			ui8Adjust--;
			if (ui8Adjust < 1)
			{
				ui8Adjust = 1;//10
			}
			//PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, ui8Adjust * ui32Load / 1000);
		}

		if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0x00)
		{
			ui8Adjust++;
			if (ui8Adjust > 254)
			{
				ui8Adjust = 254;//111
			}
			//PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, ui8Adjust * ui32Load / 1000);
		}
*/


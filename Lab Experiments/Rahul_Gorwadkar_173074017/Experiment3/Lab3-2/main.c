#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"
#include "driverlib/rom.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "inc/hw_ints.h"

#define PWM_FREQUENCY 55

#define IDLE 1
#define PRESS 2
#define RELEASE 3

uint8_t debounce_flag1=0;
uint8_t debounce_flag2=0;
uint8_t switch_status1=IDLE;
uint8_t switch_status2=IDLE;

void timer0init(void)
{
	uint32_t ui32Period_timer;

	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
	ui32Period_timer = (SysCtlClockGet() / 100) / 2;
	TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period_timer -1);
	IntEnable(INT_TIMER0A);
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	IntMasterEnable();
	TimerEnable(TIMER0_BASE, TIMER_A);
}


int main(void)
{
	volatile uint32_t ui32Load;
	volatile uint32_t ui32PWMClock;
	volatile uint8_t ui8Adjust;
	//uint8_t incdec = 0;
	ui8Adjust = 83;

	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
	SysCtlPWMClockSet(SYSCTL_PWMDIV_64);

	timer0init();

	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

	GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0);
	GPIOPinConfigure(GPIO_PD0_M1PWM0);

	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;

	GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_DIR_MODE_IN);
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

	ui32PWMClock = SysCtlClockGet() / 64;
	ui32Load = (ui32PWMClock / PWM_FREQUENCY) - 1;

	PWMGenConfigure(PWM1_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN);
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, ui32Load);
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, ui8Adjust * ui32Load / 1000);
	PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT, true);
	PWMGenEnable(PWM1_BASE, PWM_GEN_0);

	while(1)
	{
		//if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)==0x00)
		//if(incdec == 0)
		if(switch_status1 == RELEASE)
		{
			ui8Adjust--;
			if (ui8Adjust < 10)
			{
				ui8Adjust = 10; //56
				//incdec = 1;
			}
			PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, ui8Adjust * ui32Load / 1000);
			while(switch_status1 != IDLE);
		}

		//if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0x00)
		//if(incdec == 1)
		if(switch_status2 == RELEASE)
		{
			ui8Adjust++;
			if (ui8Adjust > 200)
			{
				ui8Adjust = 200; //200
				//incdec = 0;
			}
			PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, ui8Adjust * ui32Load / 1000);
			while(switch_status2 != IDLE);
		}
		SysCtlDelay(100000);
	}
}

void Timer0IntHandler(void)
{
	uint32_t key_detect;

	// Clear the timer interrupt
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

	key_detect = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0);
	if((key_detect & 0x00000001) == 0)
	{
		switch(switch_status1)
		{
			case IDLE:
				if(debounce_flag1 == 0) //debounce_flag1 will provide 10ms debounce delay for SW2
				{
					debounce_flag1 = 1;
				}
				else					//Clear debounce and declare switch press event
				{
					switch_status1 = PRESS;
					debounce_flag1 = 0;
				}
				break;
			case PRESS:
				switch_status1 = RELEASE;	//Declare switch release event
				debounce_flag1 = 0;
				break;
			case RELEASE:
				switch_status1 = RELEASE;	//Wait untill switch is released
				debounce_flag1 = 0;
 				break;
			default:
				switch_status1 = IDLE;		//Default IDLE status
				debounce_flag1 = 0;
				break;
			}
	}
	else
	{
		switch(switch_status1)
		{
			case IDLE:
				//debounce_flag = 0;		//Switch is released or not pressed. Dont do anything.
				break;
			case PRESS:
				if(debounce_flag1 == 0)		//Switch debounce to switch state from PRESS to IDLE
				{
					debounce_flag1 = 1;
				}
				else
				{
					switch_status1 = IDLE;
					debounce_flag1 = 0;
				}
				break;
			case RELEASE:
				if(debounce_flag1 == 0)		//Switch debounce to switch state from RELEASE to IDLE
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
					switch_status2 = PRESS;
					debounce_flag2 = 0;
				}
				break;
			case PRESS:
				switch_status2 = RELEASE;
				debounce_flag2 = 0;
				break;
			case RELEASE:
				switch_status2 = RELEASE;
				debounce_flag2 = 0;
				break;
			default:
				switch_status2 = IDLE;
				debounce_flag2 = 0;
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



/*

* Author: Pratik Sanjay Wagh


* Description: This code structure will assist you in completing Lab 3
* Filename: lab3.1.c

* Functions: timer0init(), detectKeyPress(), main(), Timer0IntHandler()


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

unsigned char counter=1,TempCounter =1;

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

uint8_t flag=0,press=0,prev=0;
uint8_t count=0;

void detectKeyPress(void)
{
	if((GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)==0))
	{
		if((GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)==0))
		{


	      if(prev==0)
	      {
	     	 prev=1;
             if(press==0)
	     	 flag=4;

	      }
	      else if(prev==1)
	      {

	    	  if(flag==4)
	    	  {
	    		  flag=1;
	    	  }

				count=count+1;

				if(count >= 200)
					count=200;

	    	  else if((flag==1)&&(press==1))
	    	  {
	        	   flag=2;

	           }
	    	  else if((flag==2)&&(press==2))
	    	  {
	    			  flag=3;
	    	  }

	    	  // GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 8);
	      }
		}
		else
		{

			if(flag==1)   //checking properly pressed first time or not that is in 2 intervals;
			{
				press=1;
				prev=0;

			}
			else if(flag==2)
			{
				press=2;
				prev=0;

			}
		}
	}
	else
	{

		press=0;
		prev=0;
	}
}


int main(void)
{

	unsigned int RedVal=255,BlueVal=0,GreenVal =0,state =  0;

	volatile uint32_t ui32Load;
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
		       if(flag==4)
				{

				 SysCtlDelay(38400000);

                 if((count>=140) && (flag==1))
                	 flag=3;

		         if(flag==4)
		        	 flag=5;

				}
				if(flag==1)
				{
					 if((count>=140) && (flag==1))
					     flag=3;

					press=0;
					prev=0;


					PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, 1 * ui32Load / 1000);
					PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, 1 * ui32Load / 1000);
				    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, 1 * ui32Load / 1000);

					ui8Adjust=127;

					while(flag==1)
					{
						if((count>=140) && (flag==1))
		                    flag=3;

						PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, ui8Adjust *ui32Load /1000);

						if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)==0x00)
						{
						   ui8Adjust--;
						     if (ui8Adjust < 1)
					        	{
						          ui8Adjust = 1;
						        }
						PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, ui8Adjust *ui32Load /1000);
						}

						if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0x00)
						{
						    ui8Adjust++;
						    if (ui8Adjust > 254)
						     {
						        ui8Adjust = 254;
						     }
						PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, ui8Adjust *ui32Load /1000);
						}
						SysCtlDelay(100000);
					}
					state=4;

				}
				else if(flag==2)
				{
				    press=0;
					prev=0;

					PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, 1 * ui32Load / 1000);
					PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, 1 * ui32Load / 1000);
				    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, 1 * ui32Load / 1000);

					ui8Adjust=127;
					while(flag==2)
					{

						PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, ui8Adjust *ui32Load /1000);

						if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)==0x00)
							{
								ui8Adjust--;
								if (ui8Adjust < 1)
									{
										ui8Adjust = 1;
									}
								PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, ui8Adjust *ui32Load /1000);
							}

						 if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0x00)
							{
								ui8Adjust++;
								if (ui8Adjust > 254)
								 {
									 ui8Adjust = 254;
								 }

							 PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, ui8Adjust *ui32Load /1000);

							}
							SysCtlDelay(100000);
					}
					//press=0;
					//prev=0;
					SysCtlDelay(51200000);
					state=4;
				}
				else if(flag==3)
				{
					press=0;
		            prev=0;

		            PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, 1 * ui32Load / 1000);
					PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, 1 * ui32Load / 1000);
				    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, 1 * ui32Load / 1000);

		            ui8Adjust=127;

					while(flag==3)
					{

						PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, ui8Adjust *ui32Load /1000);

						if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)==0x00)
						{
							ui8Adjust--;

							if (ui8Adjust < 1)
							 {
								ui8Adjust = 1;
							 }

						PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, ui8Adjust *ui32Load /1000);

						}

						if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0x00)
						 {
							ui8Adjust++;
							if (ui8Adjust > 254)
								{
									ui8Adjust = 254;
								}

						 PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, ui8Adjust *ui32Load /1000);

						 }
						SysCtlDelay(100000);
					}

					SysCtlDelay(51200000);
					state=4;
				}

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
			PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, GreenVal * ui32Load / 1000);
			PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, BlueVal * ui32Load / 1000);
			PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, RedVal * ui32Load / 1000);
		}

		if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)==0x00)
		{
			ui8Adjust--;
			if (ui8Adjust < 1)
			{
				ui8Adjust = 1;
			}
			PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, ui8Adjust * ui32Load / 1000);
		}

		if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0x00)
		{
			ui8Adjust++;
			if (ui8Adjust > 254)
			{
				ui8Adjust = 254;
			}
			PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, ui8Adjust * ui32Load / 1000);
		}
		SysCtlDelay(100000);
	}
}

void Timer0IntHandler(void)
{
	// Clear the timer interrupt

	TempCounter++;
	counter++;
	if(counter>254)
		counter =1;
	detectKeyPress();
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
}

/*

* Author: Pratik Sanjay Wagh
* Description: This code structure will assist you in completing Lab 2
* Filename: lab2.2.c

* Functions: setup(), pin_config(), main(), Timer0IntHandler(), detectKeyPress()


*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"

unsigned char detectKeyPress(void);
void Timer0IntHandler();

/*

* Function Name: setup()

* Input: none

* Output: none

* Description: Set crystal frequency,enable GPIO Peripherals and unlock Port F pin 0 (PF0)

* Example Call: setup();

*/
void setup(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    
	//unlock PF0 based on requirement

}

/*

* Function Name: pin_config()

* Input: none

* Output: none

* Description: Set Port F Pin 1, Pin 2, Pin 3 as output. On this pin Red, Blue and Green LEDs are connected.
			   Set Port F Pin 0 and 4 as input, enable pull up on both these pins.

* Example Call: pin_config();

*/

void pin_config(void)
{

    uint32_t ui32Period;

    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01 ;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= 0 ;

	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_0 );
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);


    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    TimerIntRegister(TIMER0_BASE, TIMER_A, Timer0IntHandler);
    ui32Period = (SysCtlClockGet() / 10) / 2;
    TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    IntMasterEnable();
    TimerEnable(TIMER0_BASE, TIMER_A);
}


uint32_t sw2status=0;
uint8_t prev=0;


int main(void)
{
    
    setup();
    pin_config();

    while(1)
    {
    }
}



void Timer0IntHandler()
{
	uint32_t flag;

	//GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 4);

	flag=detectKeyPress();

   if(flag==1)
   {
	//GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8led);

    sw2status=sw2status+1;
   }

	 TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
}

uint8_t prev1=0;

unsigned char detectKeyPress()
{
uint8_t flag;

//	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 2);

	if((GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)==0))
	{
	//	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 4);


      if(prev==0)
      {
    	//  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 8);
     	 prev=1;

    	 if(prev1==1)
    	   prev=0;

    	 flag=0;
      }
      else if(prev==1)
      {
    	  prev=0;
    	  flag=1;
    	  prev1=1;
      	 // GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 8);

      }

    return flag;
	}

	//GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 2);
	prev=0;
	flag=0;
    prev1=0;
	return flag;

}

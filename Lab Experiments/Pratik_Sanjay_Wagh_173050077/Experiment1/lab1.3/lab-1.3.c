/*

* Author: Pratik Sanjay Wagh

* Description: This code structure will assist you in completing Lab 1
* Filename: lab-1.3.c

* Functions: setup(), pin_config(), main()


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
void SW2_IntHandler(void);

/*

* Function Name: setup()

* Input: none

* Output: none

* Description: Set crystal frequency,enable GPIO Peripherals and unlock Port F pin 0 (PF0)

* Example Call: setup();

*/
void setup(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
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

	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= GPIO_LOCK_KEY;
	HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01 ;
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= 0 ;

	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_0);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_4);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1,GPIO_PIN_2,GPIO_PIN_3);
    GPIOIntDisable(GPIO_PORTF_BASE,GPIO_PIN_4);         // Disable interrupt for PC4,PC5 and PC6(in case it was enabled)
     GPIOIntClear(GPIO_PORTF_BASE,GPIO_PIN_4);           // Clear pending interrupts for PC4,PC5 and PC6
         GPIOIntRegister(GPIO_PORTF_BASE, SW2_IntHandler);   // Register our handler function for port C
         GPIOIntTypeSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_FALLING_EDGE );// Configure PC4,PC5 and PC6 for falling edge trigger
         GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_4); // Enable interrupt for  PC4,PC5 and PC6

}

uint32_t sw2;
uint32_t sw2status=0;
uint8_t flag=0;
uint8_t sw1status=0.5;
uint8_t x=0;
uint32_t ui8led=1;
int main(void)
{
    

    setup();
    pin_config();

    while(1)
    {

    	sw2=GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0);

    	if((sw2==0))
    	{
    		if(ui8led=8)
    			ui8led=2;
    		else
    		    ui8led=ui8led*2;
    	}



    	while(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0)
    	{
    		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8led);
    		SysCtlDelay(sw1status);
    	}
    	while(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==1)
    	{
    		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8led);
    		SysCtlDelay(sw1status);

    	}
    }
}

 void SW2_IntHandler(void) {

	    	x=x+1;
	    	sw1status=sw1status + (0.5*x);

}

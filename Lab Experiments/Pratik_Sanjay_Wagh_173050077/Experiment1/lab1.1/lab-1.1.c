/*

* Author: Pratik Sanjay Wagh
* Description: This code structure will assist you in completing Lab 1
* Filename: lab-1.1.c

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
GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_4);
GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);

}

uint32_t ui8SW1;
uint8_t ui8led=2;
uint8_t flag=0;

int main(void)
{
    

    setup();
    pin_config();

    flag=0;
    while(1)
    {
		


			while(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)==0)
			{
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8led);
				flag=3;
			}

			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00);
			if(flag==3)
			{
				 if(ui8led==2)
					   ui8led=8;
					else if(ui8led==8)
						ui8led=4;
					else if(ui8led==4)
					   ui8led=2;
			flag=0;
			}

	}



    }


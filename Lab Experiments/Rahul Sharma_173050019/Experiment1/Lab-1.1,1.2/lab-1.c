/*

* Author: Texas Instruments

* Editted by: Parin Chheda
          ERTS Lab, CSE Department, IIT Bombay

* Description: This code structure will assist you in completing Lab 1
* Filename: lab-1.c

* Functions: setup(), config(), main()


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
#include <time.h>


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
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= GPIO_LOCK_KEY ;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01 ;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= 0;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= GPIO_PIN_4;


    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}

int switch2_status = 0;
//int global = 1;


int main(void)
{
    int turn = 0;
    int flag = 0;
    uint8_t current = 2;
    uint8_t ui8LED = 2;

    setup();
    pin_config();

    while(1)
    {

       while(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4))
       {
           flag = 1;
       }

       if(flag == 1 && GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4))
       {
           switch2_status += 1;
           flag = 0;
       }

       while(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0))
       {
           turn = 1;
           GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8LED);

       }

       if(current == 2 && turn == 1)
       {
           ui8LED = 8;
           current = 8;
           turn =0;
       }
       else if(current == 8 && turn ==1)
       {
           ui8LED = 4;
           current = 4;
           turn = 0;
       }
       else if(current == 4 && turn ==1)
       {
           ui8LED = 2;
           current = 2;
           turn = 0;
       }
       GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);




    }
}

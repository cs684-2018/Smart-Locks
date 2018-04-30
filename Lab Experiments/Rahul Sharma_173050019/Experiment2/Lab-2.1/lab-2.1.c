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
#include <time.h>
#include "inc/tm4c123gh6pm.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"


/*

 * Function Name: setup()

 * Input: none

 * Output: none

 * Description: Set crystal frequency,enable GPIO Peripherals and unlock Port F pin 0 (PF0)

 * Example Call: setup();

 */
void pin_setup(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //unlock PF0 based on requirement

}

void timer_setup(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
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
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

}

int switch1_counter = 0;

void Timer0IntHandler(void)
{
    // Clear the timer interrupt
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    if(switch1_counter < 2 && !GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0))
    {
        switch1_counter += 1;
    }
    else if(switch1_counter == 2 && GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0))
    {
        switch1_counter = 0;
    }
}

int main(void)
{
    uint32_t ui32Period;
    uint8_t ui8LED = 2;
    int turn = 0;
    int current = 2;

    pin_setup();
    pin_config();
    timer_setup();

    ui32Period = (SysCtlClockGet() / 10) / 2;
    TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);

    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    IntMasterEnable();
    TimerEnable(TIMER0_BASE, TIMER_A);

    while(1)
    {
        while(switch1_counter == 2)
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

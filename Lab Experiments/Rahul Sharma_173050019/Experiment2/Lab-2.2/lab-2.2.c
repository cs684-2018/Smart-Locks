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

int switch2_counter = 0;
int switch2_status = 0;


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
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= GPIO_LOCK_KEY ;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01 ;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= 0;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= GPIO_PIN_4;
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

}

void Timer0IntHandler(void)
{
    // Clear the timer interrupt
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    if((switch2_counter < 2) && (!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)))
    {
        switch2_counter += 1;
    }
    else if((switch2_counter == 2) && (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)))
    {
        switch2_counter = 0;
    }
}

int main(void)
{
    uint32_t ui32Period;

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
        if(switch2_counter == 2 && GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4))
        {
            switch2_status += 1;
        }
    }
}

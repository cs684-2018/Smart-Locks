/*

* Author: Pratik Sanjay Wagh, Rahul Sharma, Rahul Gorwadkar

* Description: This code structure will assist you in completing Lab 5
* Filename: lab5.1.c

* Functions: glcd_cmd(unsigned char), uart_char(char), main(), glcd_data(unsigned char), glcd_init(), glcd_setpage(unsigned char),
  glcd_setcolumn(unsigned char), glcd_cleardisplay(), glcd_filldisplay(), glcd_logodisplay(unsigned char *), main()


*/


#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/debug.h"
#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"
#include "driverlib/rom.h"

extern const unsigned char logo[];
extern const unsigned char mickey[];

extern const unsigned char one[];
extern const unsigned char two[];
extern const unsigned char three[];
extern const unsigned char four[];
extern const unsigned char five[];
extern const unsigned char six[];
extern const unsigned char seven[];
extern const unsigned char eight[];

/*To display image include an array with hex values and index it accordingly*/

/* void glcd_cmd(cmd)
* This function sends commands to the GLCD.
* Value of RS is 0
* Command is written on data lines.
* Enable is made 1 for a short duration.
*/
void glcd_cmd(unsigned char cmd)
{
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,0x00);
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,0x00);

    /* RS = 0 */
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6,0x00);

    /* Put command on data lines */
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,cmd);
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,cmd);

    /* Generate a high to low pulse on enable */
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0,0x01);
    SysCtlDelay(100);//1340);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0,0x00);

}




/* void glcd_data(data)
* This function sends data to the GLCD.
* Value of RS is 1
* Data is written on data lines.
* Enable is made 1 for a short duration.
*/
void glcd_data(unsigned char data)
{
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,0x00);
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,0x00);

    /* RS = 1 */
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6,0x40);

    /* Put command on data lines */
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,data);
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,data);

    /* Generate a high to low pulse on enable */
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0,0x01);
    SysCtlDelay(100);//1340);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0,0x00);
}



/* void glcd_init()
* This function initializes the GLCD.
* Always call this function at the beginning of main program after configuring the port pins.
*/
void glcd_init()
{
    SysCtlDelay(6700000/50);                            // creates ~10ms delay - TivaWare fxn
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0x00);    //cbi(GPORTC_2,GLCD_RST);
    SysCtlDelay(6700000/50);
    /* Set RST */
    GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_5,0x20);

    /* Set CS1 (CS1=1 and CS2=0) The right side is selected(column>64) */
    GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_3,0x00);

    /* Select the start line */
    glcd_cmd(0xC0);
    //SysCtlDelay(6700);

    /* Send the page */
    glcd_cmd(0xB8);
//  SysCtlDelay(6700);

    /*Send the column */
    glcd_cmd(0x40);
    //SysCtlDelay(6700);

    /* Send glcd on command */
    glcd_cmd(0x3F);


    /* Initialize the right side of GLCD */
    /* Set CS2 (CS2=1 and CS1=0) The right side is selected(column>64) */
    GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_3,0x08);

    /* Select the start line */
    glcd_cmd(0xC0);
    SysCtlDelay(6700);

    /* Send the page */
    glcd_cmd(0xB8);
//  SysCtlDelay(6700);

    /*Send the column */
    glcd_cmd(0x40);
//  SysCtlDelay(6700);

    /* Send glcd on command */
    glcd_cmd(0x3F);
//  SysCtlDelay(6700);
}




/* void glcd_setpage(page)
* This function selects page number on GLCD.
* Depending on the value of column number CS1 or CS2 is made high.
*/
void glcd_setpage(unsigned char page)
{
    /* Set CS1 (CS1=1 and CS2=0) The right side is selected(column>64) */
    GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_3,0x00);
    glcd_cmd(0xB8 | page);
    SysCtlDelay(100);

    /* Set CS2 (CS2=1 and CS1=0) The right side is selected(column>64) */
    GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_3,0x08);
    glcd_cmd(0xB8 | page);
    SysCtlDelay(100);
}



/* void glcd_setcolumn(column)
* This function selects column number on GLCD.
* Depending on the value of column number CS1 or CS2 is made high.
*/
void glcd_setcolumn(unsigned char column)
{
    if(column < 64)
    {
        /* Set CS1 (CS1=1 and CS2=0) The right side is selected(column>64) */
        GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_3,0x00);
        glcd_cmd(0x40 | column);
        SysCtlDelay(100);//6700);
    }
    else
    {
        /* Set CS2 (CS2=1 and CS1=0) The right side is selected(column>64) */
        GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_3,0x08);
        glcd_cmd(0x40 | (column-64));
        SysCtlDelay(100);//6700);
    }

}



/* void glcd_cleardisplay()
* This function clears the data on GLCD by writing 0 on all pixels.
*/
void glcd_cleardisplay()
{
    unsigned char i,j;
    for(i=0;i<8;i++)
    {
        glcd_setpage(i);
        for(j=0;j<128;j++)
        {
            glcd_setcolumn(j);
            glcd_data(0x00);
        }
    }
}

/* void glcd_filldisplay()
* This function fills the data on GLCD by writing 0xFF on all pixels.
*/
void glcd_filldisplay()
{
    unsigned char i,j;
    for(i=0;i<8;i++)
    {
        glcd_setpage(i);
        for(j=0;j<128;j++)
        {
            glcd_setcolumn(j);
            glcd_data(0xFF);
        }
    }
}

void glcd_logodisplay(unsigned char *start_address)
{
    unsigned char i,j;
    for(i=0;i<8;i++)
    {
        glcd_setpage(i);
        for(j=0;j<128;j++)
        {
            glcd_setcolumn(j);
            glcd_data(~start_address[(i*128)+j]);
        }
    }
}

int main()
{
	//unsigned char i,j;
	unsigned long logo_delay;
    /* Enable all the peripherals */
    /* PORTS A,E,F,C,D,B */
	SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);


    /* Unlock pin PF0 */
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= GPIO_LOCK_KEY;    // unlocking sw2 switch
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= 0;

	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4); //This is disabling the speaker
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0x10);

	/* Configure PE5 (RST), PE0 to PE3 (D0 to D3) and PB4 to PB7(D4 to D7) as output pins */
	//GLCD D0-D3
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	//GLCD D4-D7
	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);

	/* Configure RS as output */
	//GLCD RS
	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_6);
	//GLCD Enable PIN
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);

	/* Configure CS1 or CS2 as output */
	//GLCD CS
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_3);
	//GLCD Reset
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_5);

    /*This ensures buzzer remains OFF, since PC4 when logic 0 turns ON buzzer */
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4);
    GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_4,16);

    /*initialize glcd*/
    glcd_init();

    /* Select a page and display lines on it from column 0 to 127 */
    glcd_setpage(0);
    glcd_setcolumn(0);

    glcd_cleardisplay();
    SysCtlDelay(67000000/5);
    logo_delay = 6700000;//67000000/5;
    while(1)
    {
    	//glcd_filldisplay();
        //SysCtlDelay(67000000/5);

        //glcd_cleardisplay();
        //SysCtlDelay(67000000/5);

        glcd_logodisplay(logo);
        SysCtlDelay(67000000);

        glcd_logodisplay(mickey);
        SysCtlDelay(67000000);
    }

}

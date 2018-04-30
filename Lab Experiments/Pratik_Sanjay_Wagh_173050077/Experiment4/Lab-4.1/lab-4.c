
/*

* Author: Pratik Sanjay Wagh, Rahul Sharma, Rahul Gorwadkar

* Description: This code structure will assist you in completing Lab 4
* Filename: lab-4.c

* Functions: uart_char(char), main()


*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/systick.h"
#include "driverlib/adc.h"
#include "driverlib/rom.h"
#include "driverlib/debug.h"
#include <time.h>
#include <inc/hw_gpio.h>
#include "driverlib/ssi.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"


uint32_t left_right_val[4];
uint32_t up_down_val[4];

volatile uint32_t position_avg[2];
volatile uint32_t temp[2];
volatile uint8_t digit[8];


void uart_char(char data)
{
    UARTCharPut(UART0_BASE, data);
}

int main(void)
{
    uint8_t Hpos, Vpos, HposPrev, VposPrev, UpdateCount,Terminal;
    Terminal = 0;

    SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);//This is isabling the speaker
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4);
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0x10);



     SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
     SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);

     ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
     ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH6);
     ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_CH6);
     ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_CH6);
     ADCSequenceStepConfigure(ADC0_BASE,1,3,ADC_CTL_CH6|ADC_CTL_IE|ADC_CTL_END);
     ADCSequenceEnable(ADC0_BASE, 1);

     ADCSequenceConfigure(ADC1_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
     ADCSequenceStepConfigure(ADC1_BASE, 1, 0, ADC_CTL_CH7);
     ADCSequenceStepConfigure(ADC1_BASE, 1, 1, ADC_CTL_CH7);
     ADCSequenceStepConfigure(ADC1_BASE, 1, 2, ADC_CTL_CH7);
     ADCSequenceStepConfigure(ADC1_BASE,1,3,ADC_CTL_CH7|ADC_CTL_IE|ADC_CTL_END);
     ADCSequenceEnable(ADC1_BASE, 1);

    //RG
    //SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);

    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    while (1)
    {
        ADCIntClear(ADC0_BASE, 1);
        ADCProcessorTrigger(ADC0_BASE, 1);
        ADCIntClear(ADC1_BASE, 1);
        ADCProcessorTrigger(ADC1_BASE, 1);
        while(!ADCIntStatus(ADC0_BASE, 1, false))
        {
        }
        while(!ADCIntStatus(ADC1_BASE, 1, false))
        {
        }
        ADCSequenceDataGet(ADC0_BASE, 1, left_right_val);
        ADCSequenceDataGet(ADC1_BASE, 1, up_down_val);

        position_avg[0] = (left_right_val[0] + left_right_val[1] + left_right_val[2] + left_right_val[3] + 2)/4;
        temp[0]=position_avg[0];
        position_avg[1] = (up_down_val[0] + up_down_val[1] + up_down_val[2] + up_down_val[3] + 2)/4;
        temp[1]=position_avg[1];

        //Encoding Horizontal Position
        if(temp[0] < 1501)
            Hpos = 82; //R
        else if((temp[0] > 1500) && (temp[0] < 3001))
            Hpos = 67; //C
        else if(temp[0] > 3000)
            Hpos = 76; //L
        else
            Hpos = 67;

        //Encoding Vertical Position
        if(temp[1] < 1501)
            Vpos = 66; //B
        else if((temp[1] > 1500) && (temp[1] < 3001))
            Vpos = 67; //C
        else if(temp[1] > 3000)
            Vpos = 84; //T
        else
            Vpos = 67;

        if(Terminal == 1)
        {

            digit[0]=temp[0]%10+48;
            temp[0]=temp[0]/10;
            digit[1]=temp[0]%10+48;
            temp[0]=temp[0]/10;
            digit[2]=temp[0]%10+48;
            temp[0]=temp[0]/10;
            digit[3]=temp[0]%10+48;
            temp[0]=temp[0]/10;

            digit[4]=temp[1]%10+48;
            temp[1]=temp[1]/10;
            digit[5]=temp[1]%10+48;
            temp[1]=temp[1]/10;
            digit[6]=temp[1]%10+48;
            temp[1]=temp[1]/10;
            digit[7]=temp[1]%10+48;
            temp[1]=temp[1]/10+48;

            UpdateCount++;
            //if((Hpos == HposPrev) && (Vpos == VposPrev))
            SysCtlDelay(670000);

            if((UpdateCount > 100) || ((Hpos != HposPrev) || (Vpos != VposPrev)))
            {
                UpdateCount = 0;

                UARTCharPut(UART0_BASE,1);
                UARTCharPut(UART0_BASE,2);
                UARTCharPut(UART0_BASE,'\f');

                UARTCharPut(UART0_BASE,'X');
                UARTCharPut(UART0_BASE,':');
                UARTCharPut(UART0_BASE,digit[3]);
                UARTCharPut(UART0_BASE,digit[2]);
                UARTCharPut(UART0_BASE,digit[1]);
                UARTCharPut(UART0_BASE,digit[0]);
                UARTCharPut(UART0_BASE,'\n');

                UARTCharPut(UART0_BASE,8);
                UARTCharPut(UART0_BASE,8);
                UARTCharPut(UART0_BASE,8);
                UARTCharPut(UART0_BASE,8);
                UARTCharPut(UART0_BASE,8);
                UARTCharPut(UART0_BASE,8);

                UARTCharPut(UART0_BASE,'Y');
                UARTCharPut(UART0_BASE,':');
                UARTCharPut(UART0_BASE,digit[7]);
                UARTCharPut(UART0_BASE,digit[6]);
                UARTCharPut(UART0_BASE,digit[5]);
                UARTCharPut(UART0_BASE,digit[4]);
            }
            HposPrev = Hpos;
            VposPrev = Vpos;
        }
        else
        {
            UpdateCount++;
            SysCtlDelay(670000);

            if((UpdateCount > 100) || ((Hpos != HposPrev) || (Vpos != VposPrev)))
            {
                UpdateCount = 0;
                UARTCharPut(UART0_BASE,'A');
                UARTCharPut(UART0_BASE,Hpos);
                UARTCharPut(UART0_BASE,',');
                UARTCharPut(UART0_BASE,Vpos);
                UARTCharPut(UART0_BASE,'S');
            }
            HposPrev = Hpos;
            VposPrev = Vpos;
        }
    }
}

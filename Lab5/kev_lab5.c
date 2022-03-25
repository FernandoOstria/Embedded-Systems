/*
 * Main.c
 *
 *  Created on: Mar 24, 2022
 *      Author: Kevin
 */

#include "msp.h"
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

void main(void)
{
    char text[50]= "This is a test";

    const eUSCI_UART_Config uartConfig =
    {
     EUSCI_A_UART_CLOCKSOURCE_SMCLK,
     78,
     2,
     0,
     EUSCI_A_UART_NO_PARITY,
     EUSCI_A_UART_LSB_FIRST,
     EUSCI_A_UART_ONE_STOP_BIT,
     EUSCI_A_UART_MODE,
     EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,
    };

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;        // stop watchdog timer

    UART_initModule(EUSCI_A0_BASE,&uartConfig);
    UART_transmitData(EUSCI_A0_BASE,text[1]);

    // Bottom works
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, 0x012);
    GPIO_setAsOutputPin(GPIO_PORT_P2,0x07);

    while(1)
    {
        int Switch1_Pressed = !(GPIO_getInputPinValue(GPIO_PORT_P1,GPIO_PIN1));
        int Switch2_Pressed = !(GPIO_getInputPinValue(GPIO_PORT_P1,GPIO_PIN4));


        if (Switch1_Pressed && Switch2_Pressed)
        {
            printf("Both Pressed \n");
            GPIO_setOutputHighOnPin(GPIO_PORT_P2, 0x07);
        }
        else if(Switch1_Pressed)
        {
            printf("Switch1 Pressed \n");
            GPIO_setOutputHighOnPin(GPIO_PORT_P2, 0x04);

        }
        else if (Switch2_Pressed)
        {
            printf("Switch2 Pressed \n");
            GPIO_setOutputHighOnPin(GPIO_PORT_P2, 0x01);

        }
        else
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, PIN_ALL16);

    }



}

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

const eUSCI_UART_Config uartConfig =
{
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        1,                                        // BRDIV = 1
        10,                                       // UCxBRF = 10
        0,                                       // UCxBRS = 0
        EUSCI_A_UART_NO_PARITY,                  // No Parity
        EUSCI_A_UART_LSB_FIRST,                  // LSB First
        EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
        EUSCI_A_UART_MODE,                       // UART mode
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION  // Oversampling
};

inline void uart_println(const char* str, ...)
{
  static char print_buffer[256];
  va_list lst;
  va_start(lst, str);
  vsnprintf(print_buffer, 256, str, lst);
  str = print_buffer;
  while (*str)
  {
    while (!(UCA0IFG & EUSCI_A_IFG_TXIFG));
    UCA0TXBUF = *str;
    str++;
  }
  while (!(UCA0IFG & EUSCI_A_IFG_TXIFG));
  UCA0TXBUF = '\r';
  while (!(UCA0IFG & EUSCI_A_IFG_TXIFG));
  UCA0TXBUF = '\n';
}

void main(void)
{
    /* Halting WDT  */
        MAP_WDT_A_holdTimer();

        /* Setting DCO to 12MHz */
           CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_3); // to select 3 MHZ

        /* Selecting P1.2 and P1.3 in UART mode */
        MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
                GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

             /* Configuring UART Module */
        MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig);

        /* Enable UART module */
        MAP_UART_enableModule(EUSCI_A0_BASE);

        //Setting up the Buttons as inputs and outputs
        GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, 0x012);
        GPIO_setAsOutputPin(GPIO_PORT_P2,0x07);

    while(1)
    {
        /* We want to read the 2nd bit to check whether button1 is an active high, then read 5th bit to check whether the button2 is an active high on Port 1.
           Stored input register */
        int Switch1_Pressed = !(GPIO_getInputPinValue(GPIO_PORT_P1,GPIO_PIN1));
        int Switch2_Pressed = !(GPIO_getInputPinValue(GPIO_PORT_P1,GPIO_PIN4));


        if (Switch1_Pressed && Switch2_Pressed)
        {
            uart_println("Both Pressed \n");
            GPIO_setOutputHighOnPin(GPIO_PORT_P2, 0x07);
        }
        else if(Switch1_Pressed)
        {
            uart_println("Switch1 Pressed \n");
            GPIO_setOutputHighOnPin(GPIO_PORT_P2, 0x04);

        }
        else if (Switch2_Pressed)
        {
            uart_println("Switch2 Pressed \n");
            GPIO_setOutputHighOnPin(GPIO_PORT_P2, 0x01);

        }
        else
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, PIN_ALL16);

    }



}





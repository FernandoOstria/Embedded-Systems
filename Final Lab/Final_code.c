#include "msp.h"
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <LCD.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

int miliseconds = 0;
int distance = 0;
long sensor = 0;

//#include "msp430dna.h"
//#include <intrinsics.h>
void initiateTrigger(void){


}

static void Delay(uint32_t loop)
{
    volatile uint32_t i;

    for (i = 0 ; i < loop ; i++);
}

int main(void)
{
    uint32_t i;
    uint32_t error = 0;

    MAP_WDT_A_holdTimer();    //Watchdog Timer halt
    LCD_init();
    LCD_cursorUp();


    /*         UART                      */

    //MAP_CS_setDCOFrequency(CS_DCO_FREQUENCY_12);

    //MAP_CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1); //ACLK = REF0
    //MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1); //SMCLK = 12MHZ
   // MAP_CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1); //MCLK = 12MHZ



    CS->KEY = CS_KEY_VAL;                   // Unlock CS module for register access
    CS->CTL0 = 0;                           // Reset tuning parameters
    CS->CTL0 = CS_CTL0_DCORSEL_3;           // Set DCO to 12MHz (nominal, center of 8-16MHz range)
    CS->CTL1 = CS_CTL1_SELA_2 |             // Select ACLK = REFO
            CS_CTL1_SELS_3 |                // SMCLK = DCO
            CS_CTL1_SELM_3;                 // MCLK = DCO
    CS->KEY = 0;                            // Lock CS module from unintended accesses




    // Configure GPIO (Enable ports for the LCD screen here)

    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);


    // Configure UART pins
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN4);
    MAP_GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P2, GPIO_PIN4);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN4);

    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P2, GPIO_PIN4);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P2, GPIO_PIN4);
    MAP_GPIO_interruptEdgeSelect(GPIO_PORT_P2, GPIO_PIN4, GPIO_HIGH_TO_LOW_TRANSITION);



    // Configure UART
    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST; // Put eUSCI in reset
    EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SWRST | // Remain eUSCI in reset
            EUSCI_B_CTLW0_SSEL__SMCLK;      // Configure eUSCI clock source for SMCLK
    // Baud Rate calculation
    // 12000000/(16*9600) = 78.125
    // Fractional portion = 0.125
    // User's Guide Table 21-4: UCBRSx = 0x10
    // UCBRFx = int ( (78.125-78)*16) = 2
    EUSCI_A0->BRW = 78;                     // 12000000/16/9600
    EUSCI_A0->MCTLW = (2 << EUSCI_A_MCTLW_BRF_OFS) |
            EUSCI_A_MCTLW_OS16;

    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST; // Initialize eUSCI
    EUSCI_A0->IFG &= ~EUSCI_A_IFG_RXIFG;    // Clear eUSCI RX interrupt flag
    EUSCI_A0->IE &= ~EUSCI_A_IE_RXIE;        // Disable USCI_A0 RX interrupt




    /*              TIMER A0            */
    TIMER_A0->CCTL[0]= TIMER_A_CCTLN_CCIE;       // CCR0 interrupt enabled
    TIMER_A0->CCR[0] = 1000-1;                   // 1ms at 1mhz
    TIMER_A0->CTL = TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_MC__UP | TIMER_A_CTL_CLR;                  // SMCLK, upmode

    __enable_irq();             // Enables interrupts to the system

    NVIC->ISER[1] = 1 << ((PORT2_IRQn) & 31);       // Very important to assign interrupts to the NVIC vector otherwise they would not
                                                    // considered
    NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31);
    //    __low_power_mode_3();
    //    __no_operation();

    while(1){
        LCD_clear();

        //        P2->IE &= ~BIT7;          // disable interupt
        P2->DIR |= BIT6;          // trigger pin as output
        P2->OUT |= BIT6;          // generate pulse
        Delay(100);               // for 10us
        P2->OUT &= ~BIT6;         // stop pulse
        //        P2->IE |= BIT7;          // disable interupt
        P2->IFG = 0;              // clear P2 interrupt just in case anything happened before
        P2->IES &= ~BIT4;         // rising edge on ECHO pin
        Delay(30000);             // delay for 30ms (after this time echo times out if there is no object detected)
        distance = sensor/1430;     // converting ECHO lenght into cm
        char buffer[50];
        sprintf(buffer,"%d",distance);
        LCD_display(buffer);
        uart_puts(buffer);
        if(distance < 20 && distance != 0)
            P1->OUT |= BIT0;  //turning LED on if distance is less than 20cm and if distance isn't 0.
        else
            P1->OUT &= ~BIT0;

    }

}

int uart_puts(const char *str)
{
    int status = -1;

    if (str != '\0') {
        status = 0;

        while (*str != '\0') {
            /* Wait for the transmit buffer to be ready */
            while (!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));

            /* Transmit data */
            EUSCI_A0->TXBUF  = *str;

            /* If there is a line-feed, add a carriage return */
            if (*str == '\n') {
                /* Wait for the transmit buffer to be ready */
                while (!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
                EUSCI_A0->TXBUF = '\r';
            }

            str++;
        }
    }

    return status;
}

// Timer A0 interrupt service routine
void PORT2_IRQHandler(void)
{

    if(P2->IFG & BIT4)  //is there interrupt pending?
    {
        if(!(P2->IES & BIT4)) // is this the rising edge?
        {

            TIMER_A0->CTL |= TIMER_A_CTL_CLR;   // clears timer A
            miliseconds = 0;
            P2->IES |=  BIT4;  //falling edge
        }
        else
        {
            sensor = (long)miliseconds*1000 + (long)TIMER_A0->R;    //calculating ECHO length
            //            P1->OUT ^= BIT0;
            P2->IES &=  ~BIT4;  //falling edge

        }
        P2->IFG &= ~BIT4;             //clear flag
    }
}

void TA0_0_IRQHandler(void)
{
    //    Interrupt gets triggered for every clock cycle in SMCLK Mode counting number of pulses
    miliseconds++;
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
    //    printf("milliseconds: "+miliseconds);
}

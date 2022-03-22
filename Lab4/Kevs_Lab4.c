#include <msp.h>
#include <stdint.h>
#include <stdio.h>
#define sec 3000000

void UART_Init(void){
    EUSCI_A0 ->CTLW0 = 0x0001; //reset the modulel
    EUSCI_A0 ->CTLW0 = 0x00C1;// mode clock, No parity , lSB, 8 bit , 1s top bit ..etc
    EUSCI_A0 ->BRW   = 26; // int[3000000/115200]= 26
    EUSCI_A0 ->MCTLW &= ~!0xFFF1; // USCO16= 0 , use the MSCLK 11, 3 MHZ
    P1->SEL0 |=0x0C;
    P1->SEL1 &=~0x0C;
    EUSCI_A0 ->CTLW0 &= ~0x0001; //set the module
    EUSCI_A0 ->IE &= ~0x000F; //disable interrupts
}

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
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;        // stop watchdog timer

    UART_Init();

    //We first want to Port 1, as a simple I/O using the first multiplexor containing SEL1 and SEL 0 setting them to 0's. For all Pins.
        P1->SEL0 &= ~0x00;
        P1->SEL1 &= ~0x00;
    //Then setting the direction to input to the complement 12, changing both button pins to zero.
        P1->DIR &= ~0x12;
    //Enabling Pull up resistor to enable the switch so turn all button pins to 1,
        P1->REN |= 0x012;
    //Then we want to pass voltage by selecting DVCC, so OUT is 1 for all button pins **(Why do we need voltage?)
        P1->OUT |= 0x12;


    // Set up as I/O so, SEL0 and SEL1 both as 0's
        P2-> SEL0 &= ~0x7;
        P2-> SEL1 &= ~0x7;
    //Set direction as output so, both Red, Green, Blue pins are 1's making it into 1's thus an output
        P2-> DIR |= 0x07;

        while (1) {

        /* We want to read the 2nd bit to check whether button1 is an active high, then read 5th bit to check whether the button2 is an active high on Port 1.
           Stored input register */

            int Switch1_Pressed = !(P1->IN & 0x02);
            int Switch2_Pressed = !(P1->IN & 0x10);

            _delay_cycles(sec/10); // Enough to prevent debounce
            if (Switch1_Pressed && Switch2_Pressed)
            {
                printf("Switch 1 and Switch 2 Press %s \n");
                uart_println("Switch 1 and Switch 2 Press %s \n");
                 P2->OUT |= 0x07;
            }
            else if(Switch1_Pressed)
            {
                printf("Switch 1 Press %s \n");
                uart_println("Switch 1 Press %s \n");
                 P2->OUT |= 0x04;
            }
            else if(Switch2_Pressed)
            {
                printf("Switch 2 Press %s \n");
                uart_println("Switch 2 Press %s \n");
                P2->OUT |= 0x01;
            }
            else
            {
                 P2->OUT &= 0x00;
            }


        }

}

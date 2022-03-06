#include "msp432.h"
#include<stdio.h>
#define sec 3000000

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;        // stop watchdog timer

    // Port 2 controls the RGB LED (LED2)
    P2->SEL1 &= ~0x07;
    P2->SEL0 &= ~0x07;

    P2->DIR |= 0x07;

    int counter = 0;
    int j = 0;

    for (j= 0; j <= 7; j++)
    {
        _delay_cycles(sec);
        P2->OUT = counter & 7; //mask i the counter by 7, since the highest the LED can go before repeating itself is 7. Thus we keep the right most 3 bits
        printf("%hd \n", P2->OUT );
        counter ++;
    }

    while(1)
    {
        __delay_cycles(sec); //Delay to Keep the light off, otherwise it will turn off really quick and keep the red light forever
        P2->OUT = 0x01;
        printf("%s \n", "Flash");
        __delay_cycles(2*sec);
        P2->OUT = 0x00;

     }

}

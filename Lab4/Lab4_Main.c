#include <msp.h>
#include <stdint.h>
#include <stdio.h>
#define sec 3000000


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

    P2->SEL1 &= ~0x00;
    P2->SEL0 &= ~0x00;
    P2->DIR |= 0x07;
    P2->OUT = 0x00;

    P1->SEL1 &= ~0x00;
    P1->SEL0 &= ~0x00;
    P1->DIR &= ~0x12;
    P1->REN |= 0x12;
    P1->OUT |= 0x12;

    int Test = 2;

    
    //uart_println("W",Test ); UART test line



    //Blue == 0x04    White == 0x07    RED == 0x01 : Note to self

    int j = 1;
    while (j==1)
    {
        int Switch1_Pressed = (!(P1->IN & 0x02));
        int Switch2_Pressed = (!(P1->IN & 0x10));

        //printf("%hd \n", Switch1_Pressed);   Button Flag Check
        //_delay_cycles(sec);

        if (Switch1_Pressed && Switch2_Pressed){
            P2->OUT = 0x07;
        }
        else if(Switch1_Pressed){
            P2->OUT = 0x04;
        }
        else if(Switch2_Pressed){
            P2->OUT = 0x01;
        }
        else{
            P2->OUT = 0x00;
        }

    }







}

#include "msp.h"
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <LCD.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

int main(void) {

   LCD_init();
   LCD_clear();
   LCD_cursorUp();

   int i;
   for (i = 0; i <= 10; i++ )
   {
       int unsigned number = i; //Anything >10 number bugs out b/c 4 bit
       char text2[size];
       sprintf(text2, "%d",number); // turn number to string aka characters
       LCD_display(text2);

       /*
       char text3[size] = " cm";
       LCD_display(text3);
       */

       LCD_clear();
   }

}


#ifndef LCD_H_
#define LCD_H_

#include "msp.h"
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

//LCD Pins with H/L
#define RS 1     // PORT .PIN 0
#define RW 2     // PORT. PIN 1
#define EN 4     // PORT. PIN 2
#define size 3

void delayMs(int n);
void LCD_nibble_write(unsigned char data, unsigned char control);
void LCD_command(unsigned char command);
void LCD_data(unsigned char data);
void LCD_init(void);
void LCD_clear();
void LCD_cursorUp();
void LCD_cursorDown();
void LCD_display(char *str);

void LCD_init(void) {
    GPIO_setAsOutputPin(GPIO_PORT_P4, 0xFF);/* make P4 pins output for data and controls */
    delayMs(30);                /* initialization sequence */
    LCD_nibble_write(0x30, 0);
    delayMs(10);
    LCD_nibble_write(0x30, 0);
    delayMs(1);
    LCD_nibble_write(0x30, 0);
    delayMs(1);
    LCD_nibble_write(0x20, 0);  /* use 4-bit data mode */
    delayMs(1);

    LCD_command(0x28);      /* set 4-bit data, 2-line, 5x7 font */
    LCD_command(0x06);      /* move cursor right after each char */
    LCD_command(0x01);      /* clear screen, move cursor to home */
    LCD_command(0x0F);      /* turn on display, cursor blinking */
}

/* With 4-bit mode, each command or data is sent twice with upper
 * nibble first then lower nibble.
 */
void LCD_nibble_write(unsigned char data, unsigned char control) {
    data &= 0xF0;           /* clear lower nibble for control */
    control &= 0x0F;        /* clear upper nibble for data */
    //Set RS, and RW to 00 for instruction input and to read it
    GPIO_setOutputHighOnPin(GPIO_PORT_P4,data | control | EN);
    delayMs(0);
    //I guess to refresh the display??
    P4->OUT = data;                 /* clear E */
    P4->OUT = 0;
}

void LCD_command(unsigned char command) {
    LCD_nibble_write(command & 0xF0, 0);    /* upper nibble first */
    LCD_nibble_write(command << 4, 0);      /* then lower nibble */

    if (command < 4)
        delayMs(4);         /* commands 1 and 2 need up to 1.64ms */
    else
        delayMs(1);         /* all others 40 us */
}

void LCD_data(unsigned char data) {
    LCD_nibble_write(data & 0xF0, RS);    /* upper nibble first */
    LCD_nibble_write(data << 4, RS);      /* then lower nibble  */

    delayMs(1);
}

void LCD_display(char *str)
{
    int i = 0;

    delayMs(500);

    while(str[i] != '\0') {
       LCD_data(str[i]);
       i++;
    }

    delayMs(500);
}

void LCD_clear()
{
    LCD_command(1);
}

void LCD_cursorUp()
{
    LCD_command(0x80);
}

void LCD_cursorDown()
{
    LCD_command(0xc0);
}

/* delay milliseconds when system clock is at 3 MHz */
void delayMs(int n) {
    int i, j;

    for (j = 0; j < n; j++)
        for (i = 187; i > 0; i--);      /* Delay */
}

#endif

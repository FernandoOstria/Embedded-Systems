#include<stdio.h>

int main (void) 
{
    short number;
    printf("Enter an  integer [-32768,32767]: ");
    scanf("%hd%*c", &number);

    //Nible is 4 Bytes
    // 0x000F = 0000 0000 0000 1111
    // Is and'ed to be more accurate because the number could signed, could mess up the awnser (since expands in 1's)
    short num1 = (number >> 12) & 0x000F;
    short num2 = (number >> 8) & 0x000F;
    short num3 = (number >> 4) & 0x000F;
    short num4 = (number) & 0x000F;

    printf("%hd %hd %hd %hd \n",num1,num2,num3,num4);
    return 0;
}

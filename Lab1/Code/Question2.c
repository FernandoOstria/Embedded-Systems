#include<stdio.h>

short int rotbits(short int num)
{   num = (num << 4) | (num & 0xf000) >> 12;
    return num;
}

int main(void) 
{
    short int number ;

    printf("Enter an  integer [-32768,32767]: ");
    scanf("%hd%*c", &number);    

   printf("%hd \n", rotbits(number));


    return 0;
}

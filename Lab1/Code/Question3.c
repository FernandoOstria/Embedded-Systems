#include <stdio.h>
#include <limits.h> //CHAR_BIT is defined in limits.h

unsigned setbits(unsigned x, int p, int n, unsigned y)
{
        
   int shift = p - n + 4;
    unsigned mask = (1 << n) - 1;
    return (x & ~(mask << p)) | ((y & mask) << p);

}


int main(void)
{
        unsigned x = 0xF8FF;
        unsigned y = 0XF0A2;

        int p =7;
        int n =4;


        printf("%x\n",setbits(x,p,n,y));
}

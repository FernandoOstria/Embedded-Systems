#include <stdio.h>

unsigned int setbits(unsigned, int, int, unsigned);

unsigned int setbits(unsigned x, int p, int n, unsigned y)
{
  int shift = p - n + 4;
  unsigned mask = (1 << n) - 1;
  return (x & ~(mask << shift)) | ((y & mask) << shift);
}

int main(void)
{
  unsigned x = 0XF8FF;
  unsigned y = 0XF0A2;
  int p = 7;
  int n = 4;
  
  printf("%x\n", setbits(x, p, n, y));
  
  return 0;
}
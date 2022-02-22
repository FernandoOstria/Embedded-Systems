#include <stdio.h>
//Implementation1: SWAP macro using temp variable
//#define SWAP(type,x,y) { type t; t = x; x = y; y = t;}

//Implementation2: Swap macro using BitWise XOR operations
#define SWAP(type,x,y) { a = a^b; b= b^a; a =a^b; }
//XOR works as method of swapping because of the commutavity property allows you go get back the orginal number when using XOR a second time

int main(void)
{
    int a = 6, b = 3;
    char c = 99; /* 'c' */
    float f = 42.0f ;
    long int l = 25;

    printf("int and int: types match\n");
    printf("int and int: types match\n");
    SWAP(int, a, b);
    printf("After Swap\n a = %i b = %i\n", a, b);

    printf("\nchar and float: types don't match\n");
    printf("Before SWAP\n a = %c b = %2f\n", c, f);
    SWAP(int, a, b);
    printf("After Swap\n a = %c b = %2f\n", c, f);

    printf("\nlong and int: types don't match,but implicit conversion takes place\n");
    printf("Before SWAP\n a = %i b = %li\n", a, l);
    SWAP(int, a, b);
    printf("After Swap\n a = %i b = %li\n", a, l);
    return 0;
}
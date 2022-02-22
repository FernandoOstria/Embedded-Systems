#include <stdio.h>
#define size 12

// x the number of the array
// v the array
// n is size
int binSearch(int x,int v[], int n)
{
    int *gp;

    int l = 0;
    int h = n-1;

    while (l <= n) 
    {
        int mid = l + (n-l) /2;
        // Initialzing it to the array, restarts every loop so properly point to the proper element, otherwise will constantly increase
        gp = v;
        // Now doing pointer arthimetic, to point to the middle element
        gp = gp+mid;

        //Check if x is in the middle
        if  ( *gp == x) 
        {
            return 1;
        }

        // If x greater then, move to the right
        if ( *gp < x )
        {
            l = mid + 1;
        }

        // Otherwise move to the left
        else 
            n = mid -1;
    }
    return -1;

}

int main(void)
{
    int array[size] = {2,4,6,8,9,12,16,20,32,40,78};
    int input, number;

    while(1)
    {
        printf("Enter number to search in the array: ");
        input = scanf("%i", &number);
        if (!input || input == EOF)
        return -1;
        printf("%i\n", binSearch(number, array, size));
    }
    return 0;
}

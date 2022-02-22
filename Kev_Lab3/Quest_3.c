#include <stdio.h>
#include <string.h>
#define size 3

struct Person
{
    char name[15];
    char home[30];
};

char readPerson(struct Person *a)
{
        char nme[30];
        char hme[30];

        printf("%s", "Enter name: ");
        scanf("%s", a->name);

        printf("%s", "Enter town or city: ");
        scanf("%s", a->home);
}
char searchTown(struct Person *a,char nme)
{
    for (int i = 0; i < size; i++)
    {
        if ( (*a[i].name) == nme )
        {
           printf("%s %s", "This person lives in ", a[i].home);
        }
        if ( (*(a[i].name) != nme) && (i == size) )
        {
            printf("%s", "This person could not be found");
        }
    }   

}

int main(void)
{
    struct Person Persons[size];
    int input_counter = 0;

    while (input_counter != (int)size)
    {
        readPerson(&Persons[input_counter]);
        input_counter ++;
    }
    
    printf("%s\n", "==========================================================");
    printf("%s", "Enter the name of the person you want to search for: ");
    char name[30];
    scanf("%s", &name);
    searchTown(Persons, *name);
  
    return 0;
}
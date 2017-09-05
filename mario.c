#include <cs50.h>
#include <stdio.h>


//here I declare the functions I made myself
void pyramid_building(int);
void error_function(void);

int main(void)
{
    printf("Let's build a piramid. Give me a a non-negative integer no greater than 23 for the heigth.\n");

    int levels = get_int();
    if (levels < 24 && levels > -1)
    pyramid_building(levels);
    else
    main();

}

//This is the function that builds the pyramid

int a, b, c, d;
void pyramid_building(int levels)
{
    for (a = 0; a < levels; a++)
    {
        for ( b = 0; b < (levels - (a + 1)); b++)
            printf(" ");
        {
            for (c = 0; c < a+1; c++)
                printf("#");
                printf("  ");
            {
                for (d = 0; d < a+1; d++)
                printf("#");
            }
        }
        printf("\n");
    }
}




#include <cs50.h>
#include <stdio.h>

int main(void)
{
    printf("how many minutes did you shower today?\n");
    int minutes = get_int();
    printf("you used the equivalent of %i bottles of water\n", minutes*12);
}

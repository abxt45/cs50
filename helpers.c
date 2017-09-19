/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */

#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // TODO: implement a searching algorithm
    if (n > 0)
    {
        for (int i=0; i < n+1; i++)
        {
            if (values[i] == value)
            return true;
        }
    }
return false;
}



/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // TODO: implement a sorting algorithm
    // they want a sorting algorithm wit O(n), meaning one where the amount of time
    // taken to sort relates linearly to the amount of elements. That is insertion sort.

    // i is the current element of the array we are sorting
    for(int i=0; i < n; i++)    {
        // i - j is the element we are comparing with
        for(int j=1; j == i; j++)    {
            // if i - j is greater than i it has to be shifted up 1 and i comes in its place
            if(values[i-j] > values[i])    {
                values[i-j] = values [i-j+1];
                values[i] = values[i-j];
            }

        }

    }
    return;
}

#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    string s = get_string();
    if (s != NULL)
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        char c = (s[i]);
        char d = (s[i-1]);
        if ((c != ' ' && d == ' ') || (i == 0 && c != ' '))
        {
            char u = (toupper(c));
            printf("%c", u);
        }
    }
    printf("\n");
}

// Hailey Lynn James

//
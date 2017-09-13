#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int key;
int plaintext_string_length;

void plaintext(int key);

int main(int argc, string argv[])
{
    //check if the user provided 1 argument on the command line
    if (argc == 2)
    {
    //convert the argument to an integer
        key = atoi(argv[1]);
        plaintext(key);
    }

    else
    {
        printf("Enter a single command-line argument, a non-negative integer.\n");
        return 1;
    }
}


void plaintext()
{
    printf("plaintext:");
    string plaintext = get_string();
    plaintext_string_length = strlen(plaintext);
    printf("ciphertext:");

    char ciphertext[plaintext_string_length];

    for (int plaintext_count = 0; plaintext_count < plaintext_string_length; plaintext_count++)
    {
        char plaintext_c = (plaintext[plaintext_count]);
        if (isalpha(plaintext_c))
        {

            if (isupper(plaintext_c))
            {
                ciphertext[plaintext_count] = (char) (((int)plaintext_c - 65 + key) % 26)+65;
            }

            else if (islower(plaintext_c))
            {
                ciphertext[plaintext_count] = (char) (((int)plaintext_c - 97+ key) % 26)+97;
            }
            else
            {
                ciphertext[plaintext_count] = plaintext_c;
            }
        }
        else
        {
            ciphertext[plaintext_count] = plaintext_c;
        }

    }

    for (int ciphertext_count =0; ciphertext_count < plaintext_string_length; ciphertext_count++)
    {
       //print the ciphertextarray
        printf("%c", ciphertext[ciphertext_count]);
    }

    printf("\n");

}

//test string: abcABCxyzXYZ123!@#
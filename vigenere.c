#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

string key_s;
int key_string_length;

void key_convert(string key_s);
void plaintext(int key_array[]);
int error(void);

int main(int argc, string argv[])
{
    //check if the user provided 1 argument on the command line, consisting of only alphabetical characters

    if (argc != 2)
    {
    	error();
    	return 1;
    }

    key_s = argv[1];
    key_string_length = strlen(key_s);
    for (int i=0; i < key_string_length; i++)
    {
        if (!isalpha(key_s[i]))
        {
        error();
        return 1;
        }
    }
    key_convert(key_s);
}


//this functions turns the provided key string into a zero-indexed array of ints
void key_convert()
{
    int key_array[key_string_length];
	for (int i=0; i < key_string_length; i++)
	{
		key_array[i] = (key_s[i]);

		// in case of a capital letter:
		if (key_array[i] > 96)
		{
			key_array[i] = (key_array[i] - 97);
		}

		// in case of a non-capital letter:
		else
		{
			key_array[i] = (key_array[i] - 65);
		}
	}

    plaintext(key_array);
}


//this function aks for the plaintext string, then combines it with the the key, then prints the ciphertex
void plaintext(int key_array[])
{
    printf("plaintext:");
    string plaintext = get_string();
    int plaintext_string_length = strlen(plaintext);

    printf("ciphertext:");
    char ciphertext[plaintext_string_length];

    for (int i = 0; i < plaintext_string_length; i++)
    {
       	int key = key_array[(i % key_string_length)];
        char plaintext_c = (plaintext[i]);
        if (isalpha(plaintext_c))
        {

            if (isupper(plaintext_c))
            {
                ciphertext[i] = (char) (((int)plaintext_c - 65 + key) % 26)+65;
            }

            else if (islower(plaintext_c))
            {
                ciphertext[i] = (char) (((int)plaintext_c - 97+ key) % 26)+97;
            }
            else
            {
                ciphertext[i] = plaintext_c;
            }
        }
        else
        {
            ciphertext[i] = plaintext_c;
        }

    }

    for (int ciphertext_count =0; ciphertext_count < plaintext_string_length; ciphertext_count++)
    {
       //print the ciphertextarray
        printf("%c", ciphertext[ciphertext_count]);
    }

    printf("\n");

}

//this is the function that is called if the user doesnt give good input
int error()
{
    printf("Enter only a single command-line argument consisting of only alphabetical characters.\n");
    return 1;
}

//test string: abcABCxyzXYZ123!@#
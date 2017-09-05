#include <cs50.h>
#include <stdio.h>
#include <math.h>

int check_ccnumber_length(long long cc_number);
string check_company_nr(long long cc_number);
long long luhn(long long cc_number, int cc_length);
long long luhn_var;


int main(void)
{
printf("What is your CC#?\n");
long long cc_number = get_long_long();
int cc_length = check_ccnumber_length(cc_number);
string company = check_company_nr(cc_number);
    if (cc_length == 0)
    printf ("INVALID\n");
    else
        {
            if (luhn_var == 0)
            printf ("%s\n", company);
            else
            printf ("Invalid luhn return %lli\n", luhn_var);
        }

}

// luhn
long long luhn(long long cc_number, int cc_length)
    {
    for (long long count = 0; count < cc_length; count = count + 2)
        {
        luhn_var = fmodl(cc_number, pow(100, count)) - fmodl(cc_number, (pow(10, count)) /10);
        }
    return luhn_var;
    }

//Function to check the number length

int check_ccnumber_length(long long cc_number)
    {
    if ((cc_number >= 340000000000000 && cc_number < 350000000000000) || (cc_number >= 370000000000000 && cc_number < 380000000000000))
    return 15;

    else if ((cc_number >= 5100000000000000 && cc_number < 5600000000000000) || (cc_number >= 4000000000000000 && cc_number < 5000000000000000))
    return 16;

    else if (cc_number >= 4000000000000 && cc_number < 5000000000000)
    return 13;

    else
    return 0;
    }


//Function to check the company

string check_company_nr(long long cc_number)
    {
    if ((cc_number >= 340000000000000 && cc_number < 350000000000000) || (cc_number >= 370000000000000 && cc_number < 380000000000000))
    return ("AMEX\n");

    else if (cc_number >= 5100000000000000 && cc_number < 5600000000000000)
    return ("MASTERCARD\n");

    else if (cc_number >= 4000000000000 && cc_number < 5000000000000)
    return ("VISA\n");

    else if (cc_number >= 4000000000000000 && cc_number < 5000000000000000)
    return ("VISA\n");

    else
    return ("INVALID company nr.\n");
    }




//test AMEX nr. company  378282246310005

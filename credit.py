import cs50

def luhn(cc_number, cc_length, company):
    luhn_temp = 0
    luhn_even = 0
    luhn_odd = 0
    for count in range (1, (cc_length+1)):
        luhn_temp = (cc_number%10 ** count - (cc_number%10 ** (count-1))) / 10 ** (count-1)
        if count % 2 == 0:
            luhn_temp *= 2
            if luhn_temp > 9:
                luhn_even += ((luhn_temp % 100) - (luhn_temp % 10)) / 10
                luhn_even += luhn_temp%10
            else:
                luhn_even += luhn_temp
        else:
            luhn_odd += luhn_temp
    luhn_var = (luhn_odd + luhn_even) % 10
    if luhn_var == 0:
        print ("{}".format(company));
    else:
        print ("Invalid luhn return of {}".format(luhn_var))

def check_ccnumber_length(cc_number):
    cc_length = len(str(cc_number))-2
    if ((cc_length != 13) and (cc_length != 15) and (cc_length != 16)):
        print ("INVALID card nr. length of {} digits".format(cc_length))
        quit()
    else:
        return cc_length

def check_company_nr(cc_number, cc_length):
    if ((cc_number >= 34*10**13 and cc_number < 35*10**13)
            or (cc_number >= 37*10**13 and cc_number < 38*10**13)):
        return ("AMEX")
    elif (cc_number >= 51*10**14 and cc_number < 56*10**14):
        return ("MASTERCARD")
    elif ((cc_number >= 4*10**12 and cc_number < 5*10**12)
            or (cc_number >= 4*10**15 and cc_number < 5*10**15)):
        return ("VISA")
    else:
        print ("INVALID card nr.")
        quit()

print ('What is your CC#?')
cc_number = cs50.get_float()
cc_length = int(check_ccnumber_length(cc_number))
company = check_company_nr(cc_number, cc_length)
luhn(cc_number, cc_length, company)

# 378282246310005
# test AMEX nr. luhn: 378282246310005
# test AMEX nr. company  378282246310005
# identifies 378282246310005 as AMEX
# identifies 371449635398431 as AMEX
# identifies 5555555555554444 as MASTERCARD
# identifies 5105105105105100 as MASTERCARD
# identifies 4111111111111111 as VISA
# identifies 4012888888881881 as VISA
# identifies 1234567890 as INVALID
# rejects a non-numeric input of "foo"
# rejects a non-numeric input of ""
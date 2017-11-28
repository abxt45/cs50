import cs50
import sys

keystring=sys.argv[1]
keylength = len(sys.argv[1])
if keylength != 2:
    print("provide at one command-line argument")
keyints = []
for i in range(keylength):
    j = ord(keystring[i])
    if (j >= 65 and j <= 90):
        keyints.append((j -65))
    elif (j >= 97 and j <= 122):
        keyints.append((j -97))
    else:
        print("arg is not alphabetic")
        quit
print("plaintext: ", end="")
plaintext = cs50.get_string()
ciphertext = []
keyintnr=0
for i in range(len(plaintext)):
    key = keyints[i%keylength]
    j = ord(plaintext[i])
    if (j >= 65 and j <= 90):
        ciphertext.append(chr(((j -65 + key) %26)+65))
    elif (j >= 97 and j <= 122):
        ciphertext.append(chr(((j -97 + key) %26)+97))
    else:
        ciphertext.append(chr(j))
print("ciphertext: ", end="")
print("".join(ciphertext))
#test string: abcABCxyzXYZ123!@#
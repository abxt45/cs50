import cs50

def main():
    num_levels = get_pos_int()
    #this is a loop for each level of the pyramid. #the pyramid is written top to bottom. The number of ## printed in a line is the line count, starting from 1
    for level in range(num_levels):
        print(' ' * (num_levels-level+1), end="")
        print("#" * (level+1), end="")
        print('  ', end="")
        print("#" * (level+1), end="")
        print(' ' * (num_levels-level+1), end="")
        print('')


def get_pos_int():
    while True:
        print("Let's build a piramid. Give me a a non-negative integer no greater than 23 for the heigth.")
        n = cs50.get_int()
        if n > 0 or n < 23:
            break
    return n

if __name__ == "__main__":
    main()
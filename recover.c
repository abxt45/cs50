#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover infile\n");
        return 1;
    }

    // open memory card file
    FILE *filepointer_raw = fopen(argv[1], "rb");
    if (filepointer_raw == NULL)
        return 1;

    // create a buffer on the heap. an array wont work we dont know the file sizes
    unsigned char buffer[512] = {0};

    // initialize file number, name and storage for the first file and open it.
    int filenumber = 0;
    char filename[8] = {0};
    FILE* filepointer_jpg = fopen("000.jpg", "wb");
    if (filepointer_jpg == NULL)
        return 4;

    // loop as long as fread does not return anything other than 1 (0 = EOF).
    // This reads the file into the buffer
    while(fread(buffer, sizeof(buffer), 1, filepointer_raw) == 1)
    {
        //printf("%i %i %i %i\n", buffer[0], buffer[1], buffer[2], buffer[3]);

        // look for chunk that starts with header bytes.
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //if filenumber != 0, close previous file
            if(filenumber!=0)
                fclose(filepointer_jpg);

            //open a file and assign filename
            sprintf(filename, "%03d.jpg", filenumber);
            filepointer_jpg = (fopen(filename, "wb"));
            if (filepointer_jpg == NULL)
            return 5;

            //write to the file from the buffer
            fwrite(buffer, sizeof(buffer), 1, filepointer_jpg);
            filenumber++;
        }

        // if its not a header and the first file has been created,
        // add the bytes to the currently open file.
        else if(filenumber != 0)
        {
            //fopen(filename, "a");
            if (filepointer_jpg != NULL)
            //return 6;
            fwrite(buffer, sizeof(buffer), 1, filepointer_jpg);
        }
    }

    // if the program comes here either no jpg header was ever found or the function has reached EOF.
    return 0;
}
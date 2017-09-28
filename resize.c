/**
 * Copies a BMP piece by piece, just because.
 */

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./copy float infile outfile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // remember float value and check if float value is a float and is between 0 and 100
    char *f_str = argv[1];
    float f; char c;
    if ((sscanf(f_str, " %f %c", &f, &c) != 1) || f < 0.0 || f > 100.0)
        {
            fprintf(stderr, "Provide a resize factor between 0 and 100.\n");
            return 2;
        }

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 4;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 5;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 6;
    }

    //save old height and widht
    int biWidth_old = bi.biWidth;
    int biHeight_old = bi.biHeight;
    int scanline_old = biWidth_old * sizeof(RGBTRIPLE);
    int padding_old = (4 - scanline_old % 4) % 4;
    //int scanline_full_old = (scanline_old + padding_old);


    //change height and widht in headerfile according to provided factor f
    bi.biWidth *= f;
    bi.biHeight *= f;


    //determine size of the new scanline, padding for new scanlines, size of full scanline, size of headers
    int scanline = (bi.biWidth * sizeof(RGBTRIPLE));
    int padding = (4 - scanline % 4) % 4;
    int scanline_full = (scanline + padding);
    int headers_size = (sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));

    //change filesize and imagesize according to provided factor f
    bi.biSizeImage = scanline_full * abs(bi.biHeight);
    bf.bfSize = headers_size + bi.biSizeImage;

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(biHeight_old); i < biHeight; i++)
    {
        // temporary storage
        RGBTRIPLE triple[biWidth_old];

        // iterate over pixels in scanline in infile
        for (int j = 0; j < biWidth_old; j++)
        {
            // read RGB triple from infile
            fread(&triple[j], sizeof(RGBTRIPLE), 1, inptr);

            // iterate over pixels in scanline in outfile
            for (int k = 0; k < f; k++)
                {
                    // write RGB triple to outfile
                    fwrite(&triple[j], sizeof(RGBTRIPLE), 1, outptr);
                }
        }
        //skip over padding if any;
        fseek(inptr, padding_old, SEEK_CUR);

        // write padding if any
        for (int l = 0; l < padding; l++)
        {
            fputc(0x00, outptr);
        }


        //write newly written scanline f additional times. m = 1 because
        //the scanline has already been written once, so if f==1
        //this loop should be skipped. If f < ) this will also be skipped.
        for (int m = 1; m < f; m++)
        {
            for (int n = 0; n < biWidth_old; n++)
            {
                for (int o = 0; o < f; o++)
                fwrite(&triple[n], sizeof(RGBTRIPLE), 1, outptr);
            }

            //write padding if any
            for (int p = 0; p < padding; p++)
            {
                fputc(0x00, outptr);
            }
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;

}
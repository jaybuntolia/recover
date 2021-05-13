#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    unsigned char buffer[512] = {0};
    char jpgName[8] = {0};
    int jpgcounter = 0;
    FILE *outptr = NULL;

    // open memory card file
    FILE *inptr = fopen("card.raw", "rb");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", "card.raw");
        return 1;
    }

    // Start reading
    while (fread(buffer, sizeof(buffer), 1, inptr) == 1)
    {

        // Find beginning of jpg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] == 0xe0 || buffer[3] == 0xe1))
        {
            if (NULL != outptr)
            {
                fclose(outptr);
                outptr = NULL;
            }

            // Create filename for jpegs
            sprintf(jpgName, "%03d.jpg", jpgcounter);

            // Open a new file with name defined by sprintf
            outptr = fopen(jpgName, "wb");
            if (NULL == outptr)
            {
                printf("Could not open %s.\n", jpgName);
                return 1;
            }

            // Write into the outfile from buffer
            fwrite(buffer, sizeof(buffer), 1, outptr);

            // Increment counter for jpegs
            jpgcounter++;
        }
        else
        {
            // Continue writing to old file if it is not beginning of file
            if (NULL != outptr)
            {
                fwrite(buffer, sizeof(buffer), 1, outptr);
            }
        }
    }
    if (NULL != outptr)
    {
        fclose(outptr);
    }
    if (NULL != inptr)
    {
        fclose(inptr);
    }
    return 0;
}
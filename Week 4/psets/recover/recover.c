#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    //input validation
    if (argc != 2)
    {
        printf("provide one file\n");
        return 1;
    }

    //open memory card
    FILE *raw_file = fopen(argv[1], "r");
    if (raw_file == NULL)
    {
        return 1;
    }

    //create buffer
    int block_size = 512; //block of 512 bytes
    uint8_t buffer[block_size];

    //create output file memory allocation
    FILE *output_image = NULL;

    //create memory allocation for name
    char *image_name = malloc(8 * sizeof(char)); //3 digits for name, 4 digits for extension and 1 for nul character
    if (image_name == NULL)
    {
        return 1;
    }

    //count to name the output image
    int image_count = 0;

    //read the blocks of memory searching for jpg
    while (fread(buffer, 1, block_size, raw_file) == block_size)
    {
        //check for jpg signature
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //name the file
            sprintf(image_name, "%03i.jpg", image_count);

            //closing previous image if open
            if (image_count > 0)
            {
                fclose(output_image);
            }

            //open the output file
            output_image = fopen(image_name, "w");

            //copy the jpg to the new file
            fwrite(buffer, 1, block_size, output_image);

            //increase image count
            image_count++;
        }
        else if (image_count > 0)
        {
            //continue writing to same file
            fwrite(buffer, 1, block_size, output_image);
        }
    }

    //free memory
    free(image_name);

    //close the files
    fclose(output_image);
    fclose(raw_file);

    return 0;
}
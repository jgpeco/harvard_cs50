#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    //checking to see if we have the right pattern to use the program
    if(argc != 2)
    {
        fprintf(stderr, "The usage of the program is ./recover.c image\n");
        return 1;
    }

    //openin the file given and checking if it is valid
    FILE *input = fopen(argv[1], "r");

    if(input == NULL)
    {
        fprintf(stderr, "Please, provide a valid file\n");
        return 1;
    }

    //creating filename and file output
    char filename[8];
    FILE *output = NULL;


    //creating arra with 512 bytes
    BYTE buffer[512];

    //variable to keep track of number of images
    int imageCounter = 0;



    //reading 512 bytes
    while (fread(buffer, sizeof(BYTE)*512, 1, input))
     {

        //if found new jpg header
        if((buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0))
        {
            //check to see if we already have an image, if so, this is the start of another one
            if(output != NULL && imageCounter > 0)
            {
                fclose(output);
            }

            //create new file
            sprintf(filename, "%03i.jpg", imageCounter);
            output = fopen(filename, "w");

            //check to see if the image was created
            if(output == NULL)
            {
                fclose(input);
                fprintf(stderr, "Couldn't create new image");
                return 1;
            }

            imageCounter++;

        }

        //writing in the output (if we have one)
        if(output != NULL)
        {
            fwrite(buffer, sizeof(BYTE)*512, 1, output);
        }

     }

    //close last files
    fclose(output);
    fclose(input);

    //finish program
    return 0;
}

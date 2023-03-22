#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t BYTE;
#define CHUNK_SIZE 512

int main(int argc, char *argv[])
{
    if (argc != 2) //Checks if there's more or less than 2 arguments.
    {
        printf("%s\n", "Usage: ./recover image");
        return 1;
    }
    FILE *file = fopen(argv[1], "r"); //Opens 'card.raw'.
    if (!file) //Checks if the program was able to open 'card.raw'.
    {
        printf("%s\n", "Couldn't open file");
        return 1;
    }
    BYTE chunk[512]; //This array represents a hypothetical space for each JPEG file.
    int JPEG_count = 0; //This variable will go up if the conditions for a new JPEG are met (BYTES 0, 1, 2 and 3).
    char filename[8]; //This array will store the name of every output file as a string.
    FILE *output = NULL; //We create the root output file from which every filename file will come.
    while (1) //Seems like it will go on forever but it only will until the drive is done.
    {
        size_t bytesRead = fread(chunk, sizeof(BYTE), CHUNK_SIZE, file); //This size_t stores the numerical output of every fread.
        if (bytesRead == 0 && feof(file)) //If the end of the drive has been reached...
        {
            break; //stop.
        }
        bool isJPEG = (chunk[0] == 0xff && chunk[1] == 0xd8 && chunk[2] == 0xff && (chunk[3] & 0xf0) == 0xe0); //Bool can only be T or F.
        if (isJPEG && output != NULL) //If we are still writing into output and the bool is true, we document it and close said output.
        {
            fclose(output);
            JPEG_count++;
        }
        if (isJPEG) //If a new JPEG has been found...
        {
            sprintf(filename, "%03i.jpg", JPEG_count); //A new name for this output file is written as a string.
            output = fopen(filename, "w"); //And we convert our output file into 'filename.jpg'.
        }
        if (output != NULL) //If output now equals 'filename.jpg'...
        {
            fwrite(chunk, sizeof(BYTE), bytesRead, output); //The read info. is written into it.
        }
    }
    fclose(output);
    fclose(file);
    return 0;
}
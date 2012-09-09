/* Before we write the screen display, need an intermediate test, so this file will contain code to write the contents
   of the memory mapped screen ram to file.

   The plan is to write entire screenfulls at a time as an ascii text file with each new screen appended to the end

   We can then create a simple Python program to display this graphically

   The final step is to use some kind of interprocess communication to display the screen in real time

*/

#include <stdio.h>


/*
filename:      The file to dump/append data to
RAM:           Pointer to the main system RAM structure
screenStart:   Position of screen data in RAM (its address basically)
screenSize:    Ending position of screen data in RAM
*/

// TODO: Right now we open and close the file for each write, better to open once, then close at the very end
// TODO: Need to decide what format we want the data - right now we print ints into an ASCII file
//       Each byte/int in the file represents 8 pixels on the screen, so 031 is 00011111 which means we have 3 black
//       and four white pels.
int screenMemDump(char *fileName, unsigned char *RAM, unsigned int screenStart, unsigned int screenSize)
{
    FILE *fp;
    int offset;

    fp = fopen(fileName,"a+");
    if (fp == NULL)
    {
        printf("ERROR OPENING FILE");
        return -1;
    }

   //fwrite(RAM + screenStart, 1, screenSize, fp);

   for (offset= screenStart; offset < screenStart + screenSize; offset++)
   {

      //printf("SCREEN %d\n",*(RAM+offset));
      fprintf(fp,"%d\n",*(RAM+offset));

   }
   fclose(fp);
    return 0;
}


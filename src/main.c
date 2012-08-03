#include <stdio.h>
#include <stdlib.h>

/*
   TODO: Need to split the read into high byte and low byte - right now it overflows trying to fit a word into a char.

*/


int binStr2Dec(char *binString, int nBits);

unsigned char ROM[50000];

int main()
{
    int rc, i;

    rc = readHackFile(ROM);

    for (i=0; i< 100; i++)
    {
       printf("ROM[%d] = %X\n", i, ROM[i]);
    }

    return 0;
}

int readHackFile(unsigned char *ROM)
{
   int converted;
   int num = 0;
   char tempBuffer[50];
   FILE *fp;
   fp = fopen("testReadHack.hack","r");

   if (!fp)
   {
      printf("ERROR: FAILED TO OPEN FILE");
      return -1;
   }

   converted = 0;
   while(fgets(tempBuffer, 20, fp))
   {
      converted = binStr2Dec(tempBuffer,16);
      *ROM++ = converted;
   }

   return 0;

}


int binStr2Dec(char *binString, int nBits)
{
   char *currentC;
   long sum = 0;
   long base = 0;
   int count = 0;

   base = 2 << (nBits - 2);

   while (count < nBits)
   {
      currentC = *binString++;
      sum += atol(&currentC)*base;
      base /= 2;
      count++;
   }
   return sum;
}

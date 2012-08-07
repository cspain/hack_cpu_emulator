#include <stdio.h>
#include <stdlib.h>

#define BIG_ENDIAN 1  // Store MSB at lowest address

/*
   TODO: Need to split the read into high byte and low byte - right now it overflows trying to fit a word into a char.

*/


int binStr2Dec(char *binString, int nBits);



int test_main()
{
    int rc, i;
    unsigned char ROM[0xFFFF];
    memset(ROM, 0xFF, sizeof(ROM));

    rc = readHackFile(ROM);

    for (i=0; i< 1000; i++)
    {
        printf("ROM[%d] = %2X\n", i, ROM[i]);
    }

    return 0;
}

int readHackFile(unsigned char *ROM)
{
    int converted;
    int num = 0;
    unsigned char highByte;
    unsigned char lowByte;
    char tempBuffer[50];
    FILE *fp;
    fp = fopen("src/testReadHack.hack","r");

    if (!fp)
    {
        printf("ERROR: FAILED TO OPEN FILE\n");
        return -1;
    }

    converted = 0;
    while(fgets(tempBuffer, 20, fp))
    {
        converted = binStr2Dec(tempBuffer,16);
        highByte = (converted >> 8) & 0xFF;
        lowByte = converted & 0xFF;

        //printf("high byte: %X, low byte %X\n",highByte, lowByte);

        if (BIG_ENDIAN)
        {
            *ROM++ = highByte;
            *ROM++ = lowByte;
        }
        else
        {
            *ROM++ = lowByte;
            *ROM++ = highByte;
        }

    }

    return 0;

}


int binStr2Dec(char *binString, int nBits)
{
    int *currentC;
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

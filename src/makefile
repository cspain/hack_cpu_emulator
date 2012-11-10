OBJS = CPU.o loadROM.o storeScreenMem.o
CC = gcc
DEBUG = -Wall
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)


cpuEm: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o cpuEm

CPU.o: CPU.c hack_instruction_set.h
	$(CC) $(CFLAGS) CPU.c

loadROM.o: loadROM.c
	$(CC) $(CFLAGS) loadROM.c

storeScreenMem.o: storeScreenMem.c
	$(CC) $(CFLAGS) storeScreenMem.c



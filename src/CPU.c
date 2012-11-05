// Started 7/9/12 Hack CPU Emulator

// Loading immediate values  - get from the opcode A = MEMORY[pc+1] etc.
// The only way to get values into the system is via the @ A instruction

#define DEBUG 1
#define DEBUG_REGS 1
#define OPCODE_OFFSET 0x0 // not all opcodes are used so we need an offset into the valid ones
#define MAX_OP 0x400 + 0xFF// Number of possible opcodes
#define NUM_EXEC 65535 // Limit the number of executions (only for debug)
#define SCREEN 0x4000 // Screen mapped to this address
#define SCREEN_SIZE 0x2000 // Size of screen map
#define KBD 0x6000 // Keyboard mapped to this address

#define DUMP_SCREEN_MEM 1//Turn on or off the screen memory dump

#define F_ZERO       (1 << 0)
#define F_CARRY      (1 << 1)
#define F_SIGN       (1 << 2)
#define F_OVERFLOW   (1 << 3)
#define F_PARITY     (1 << 4)

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "hack_instruction_set.h"

#if (DEBUG > 0)
#define DBG(_fmt_, ...) printf(_fmt_,## __VA_ARGS__)
#else
#define DBG(_fmt_, ...)
#endif


#if (DEBUG_REGS > 0)
#define DBGREG(_fmt_, ...) printf(_fmt_,## __VA_ARGS__)
#else
#define DBGREG(_fmt_, ...)
#endif

void execute(void);
extern int readHackFile(unsigned char *ROM);

//extern int readHackFile(unsigned *char ROM);
unsigned char RAM[0xFFFF];
//char RAM[0xFFFF ] = {0};
/*
unsigned char RAM[0xFFFF] = {0x02, 0xE3, 0x6, 0x5, 0x21, 0xFF, 0x9F, 0x32, 0xCB, 0x7C, 0x20, 0xFB, 0x21, 0x26, 0xFF, 0x0E,
                             0x11, 0x3E, 0x80, 0x32, 0xE2, 0x0C, 0x3E, 0xF3, 0xE2, 0x32, 0x3E, 0x77, 0x77, 0x3E, 0xFC, 0xE0,
                             0x47, 0x11, 0x04, 0x01, 0x21, 0x10, 0x80, 0x1A, 0xCD, 0x95, 0x00, 0xCD, 0x96, 0x00, 0x13, 0x7B,
                             0xFE, 0x34, 0x20, 0xF3, 0x11, 0xD8, 0x00, 0x06, 0x08, 0x1A, 0x13, 0x22, 0x23, 0x05, 0x20, 0xF9,
                             0x3E, 0x19, 0xEA, 0x10, 0x99, 0x21, 0x2F, 0x99, 0x0E, 0x0C, 0x3D, 0x28, 0x08, 0x32, 0x0D, 0x20,
                             0xF9, 0x2E, 0x0F, 0x18, 0xF3, 0x67, 0x3E, 0x64, 0x57, 0xE0, 0x42, 0x3E, 0x91, 0xE0, 0x40, 0x04,
                             0x1E, 0x02, 0x0E, 0x0C, 0xF0, 0x44, 0xFE, 0x90, 0x20, 0xFA, 0x0D, 0x20, 0xF7, 0x1D, 0x20, 0xF2,
                             0x0E, 0x13, 0x24, 0x7C, 0x1E, 0x83, 0xFE, 0x62, 0x28, 0x06, 0x1E, 0xC1, 0xFE, 0x64, 0x20, 0x06,
                             0x7B, 0xE2, 0x0C, 0x3E, 0x87, 0xF2, 0xF0, 0x42, 0x90, 0xE0, 0x42, 0x15, 0x20, 0xD2, 0x05, 0x20,
                             0x4F, 0x16, 0x20, 0x18, 0xCB, 0x4F, 0x06, 0x04, 0xC5, 0xCB, 0x11, 0x17, 0xC1, 0xCB, 0x11, 0x17,
                             0x05, 0x20, 0xF5, 0x22, 0x23, 0x22, 0x23, 0xC9, 0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B,
                             0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,
                             0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC,
                             0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E, 0x3c, 0x42, 0xB9, 0xA5, 0xB9, 0xA5, 0x42, 0x4C,
                             0x21, 0x04, 0x01, 0x11, 0xA8, 0x00, 0x1A, 0x13, 0xBE, 0x20, 0xFE, 0x23, 0x7D, 0xFE, 0x34, 0x20,
                             0xF5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20, 0xFB, 0x86, 0x20, 0xFE, 0x3E, 0x01, 0xE0, 0x50
                            };
*/


#define ROM_END 0x7FF
#define RAM_START ROM_END + 1
#define RAM_END   ROM_END + 0xFFFF


/***********************************

Implement all op code functions

TODO: add clock cycle update to each function
TODO: m references are memory writes and should be accessed by
MMU(A) not 'm'
WARNING: 1) In some of these instructions we may be modifying the register in the first
    part, yet we need it unmodified in the second. e.g. {cpu._r.m--; cpu._r.d = cpu._r.d = -cpu._r.m}
         2) Same as 1, but we changed either memory or the A register!

IDEA: Write a test to check every function  - probably start with a list of all opcodes and run it.
IDEA: May be better in the multiple reg updates to have temp = A op B; A = B = temp;
************************************/

struct registers
{
    unsigned int
    a,   // address register
    d,   // data register
    sp,  // stack pointer
    pc,  // program counter         i,
    ms,   // time taken for last instruction
    t;   // total time CPU has run

    unsigned char st;  // status register - (0)Z-ero, (1)C-arry, (2)S-ign, (3)O-verflow, (4)P-arity

};

/******************************************************
 *
 *           ---CENTRAL PROCESSING UNIT----
 *
 *   Main CPU Struct that controls everything and
 *   maintains timings and internal state of the system
 *
 *
 *
 *****************************************************/

struct cpu_s
{
    struct registers _r;                       // CPU state registers
    //struct operations _ops;                  // Operations functions
    int _clock;                                // Timing details
    void (*reset)(void);                       // Reset function
    void (*map[MAX_OP])(void);                    // Array of function pointers to operations - organized by opcode
    void (*execute)(void);                     // Execute the next instruction pointed to by the program counter pc

} cpu; // now have a global cpu object


/******************************************************
 *
 *           ---MEMORY MANAGEMENT  UNIT----
 *
 *   Main Struct that controls memory access
 *
 *
 *
 *
 *****************************************************/


/* Memory Management Unit
   Note: the hack cpu has a separate ROM for the code and a RAM bank for data storage/manipulation.
   In other words, code and data do not share the same RAM

*/


/* create a large memory bank of 65536 bytes = 64 KB
   Also, assume that program instructions are stored in
   a ROM which is mapped from 0 to 2047 (0x7FF)
   After this is mapped in the RAM


*/

struct MMU_s
{
    void (*wb) (unsigned int address, unsigned char value);    // Write byte
    void (*ww) (unsigned int address, unsigned int value);     // Write word

    char (*rb) (int address);                // Read byte
    int (*rw) (int address);                 // Read word

} MMU;

/* Read in the machine code from a file containing binary numbers in a string */
/*
void readROM()
{
    FILE *fp;

    fp = fopen("hackROM.hack","r");

    fread(fp,);
}
*/

/* note: have to fill out the MMU struct with the following functions */

void writeByte (unsigned int address, unsigned char value)
{
    int mask = 0xFFFF; // todo: set the correct mask

    assert(address < 0xFFFF);

    address &= mask;

    /* make sure we don't try and write to ROM */
    if (address < ROM_END)
    {
        printf("writeByte ERROR: ATTEMPTING TO WRITE TO READ ONLY ROM\n");
        //return;
    }

    if (address > RAM_END)
    {
        printf("writeByte ERROR: ATTEMPTING TO WRITE PAST MAPPED MEMORY RANGE\n");
        //return;
    }

    /* Now write the data to the RAM */
    printf("Writing %x to RAM[%x]\n\n",value,address);
    RAM[address] = value;
}

void writeWord (unsigned int address, unsigned int value)
{
    char byteHigh;
    char byteLow;

    byteHigh = (unsigned char)(value >> 8 );
    byteLow = (unsigned char)(value & 0xFF);

    /* TODO: check endianness for hack cpu, assume little for now */

    writeByte(address, byteLow);
    writeByte(address + 1, byteHigh);
}

char readByte (int address)
{
    /* don't care if we're reading from ROM or RAM, just make sure we're in range */
    if (address > RAM_END)
    {
        printf("readByte ERROR: ATTEMPTING TO READ OUTSIDE RANGE\n");
        return -1;
    }

    DBG("reading byte from address %1x, returning %1x\n",address,(unsigned)RAM[address]);
    return RAM[address];
}


int readWord (int address)
{
    unsigned char byteHigh;
    unsigned char byteLow;
    unsigned int combined;
    DBG("reading word at address %d\n",address);
    byteHigh = readByte(address);
    byteLow = readByte(address + 1);

    DBG("byte high %x, byte low %x\n", (unsigned)byteHigh, (unsigned)byteLow);

    combined = (byteHigh << 8) | byteLow;

    DBG("_______________ combined: %x _________________\n",(unsigned)combined);

    return combined;
}


void initMMU(void)
{
    MMU.wb = writeByte;
    MMU.ww = writeWord;

    MMU.rb = readByte;
    MMU.rw = readWord;
}

/* --------------------------------------------------------- */



/******************************************************
 *
 *           ---OPERATIONS/ALU FUNCTIONS----
 *
 *   A function for every (valid) opcode
 *
 *
 *
 *
 *****************************************************/


// put these in an array of function pointers, then opcode 0x8 will call op[8] which will be the correct function for that opcode


void NOP()
{
    /* Does not do anything, though we should update the timer here when implemented */
    DBG("NOP NOP NOP NOP\n");
    int a = 0;
    if (a)
    {
        a = 0;
    }

}

void LOAD_A()
{
    cpu._r.a = MMU.rw(cpu._r.pc);    //:A = number
    DBGREG("--------->LOADING regA with %x ************\n",cpu._r.a);
}
void SET_M_zero()
{
    MMU.ww(cpu._r.a, 0);    //:M = 0
    cpu._r.st |= F_ZERO;    // Set Z

}
void SET_M_one()
{
    MMU.ww(cpu._r.a, 1);    //:M = 1
    cpu._r.st &= ~F_ZERO;   // Clear Z

}
void SET_M_negOne()
{
    MMU.ww(cpu._r.a, -1);    //:M = -1
    cpu._r.st &= ~F_ZERO;   // Clear Z
    cpu._r.st |= F_SIGN;   // Set S
}
void MOV_M_D()
{
    MMU.ww(cpu._r.a, cpu._r.d);    //:M = D
}
void MOV_M_A()
{
    MMU.ww(cpu._r.a, cpu._r.a);    //:M = A
}
void MOV_M_notD()
{
    MMU.ww(cpu._r.a,!cpu._r.d);    //:M = !D
}
void MOV_M_notA()
{
    MMU.ww(cpu._r.a,!cpu._r.a);    //:M = !A
}
void MOV_M_negD()
{
    MMU.ww(cpu._r.a, -cpu._r.d);    //:M = -D
}
void MOV_M_negA()
{
    MMU.ww(cpu._r.a,-cpu._r.a);    //:M = -A
}
void MOV_M_incD()
{
    MMU.ww(cpu._r.a, cpu._r.d + 1);    //:M = D + 1
}
void MOV_M_incA()
{
    MMU.ww(cpu._r.a, cpu._r.a + 1);    //:M = A + 1
}
void MOV_M_decD()
{
    MMU.ww(cpu._r.a, cpu._r.d - 1);    //:M = D - 1
}
void MOV_M_decA()
{
    MMU.ww(cpu._r.a, cpu._r.a - 1);    //:M = A-1
}
void MOV_M_DaddA()
{
    MMU.ww(cpu._r.a, cpu._r.d + cpu._r.a);    //:M = D + A
}
void MOV_M_DsubA()
{
    MMU.ww(cpu._r.a, cpu._r.d - cpu._r.a);    //:M = D - A
}
void MOV_M_AsubD()
{
    MMU.ww(cpu._r.a, cpu._r.a - cpu._r.d);    //:M = A - D
}
void MOV_M_DandA()
{
    MMU.ww(cpu._r.a, cpu._r.d & cpu._r.a);    //:M = D & A
}
void MOV_M_DorA()
{
    MMU.ww(cpu._r.a, cpu._r.d | cpu._r.a);    //:M = D | A
}
void IDT_M()
{
    MMU.ww(cpu._r.a, MMU.rw(cpu._r.a));    //:M = M
}
void INV_M()
{
    MMU.ww(cpu._r.a, !MMU.rw(cpu._r.a));    //:M = !M
}
void NEG_M()
{
    MMU.ww(cpu._r.a, -MMU.rw(cpu._r.a));    //:M = -M
}
void INC_M()
{
    MMU.ww(cpu._r.a, MMU.rw(cpu._r.a) + 1);    //:M = M+1
}
void DEC_M()
{
    MMU.ww(cpu._r.a, MMU.rw(cpu._r.a) - 1);;    //:M = M-1
}
void MOV_M_DaddM()
{
    MMU.ww(cpu._r.a, cpu._r.d + MMU.rw(cpu._r.a));    //:M = D+M
}
void MOV_M_DsubM()
{
    MMU.ww(cpu._r.a, cpu._r.d - MMU.rw(cpu._r.a));    //:M = D-M
}
void SUB_M_D()
{
    MMU.ww(cpu._r.a, MMU.rw(cpu._r.a) - cpu._r.d);    //:M = M-D
}
void MOV_M_DandM()
{
    MMU.ww(cpu._r.a, cpu._r.d & MMU.rw(cpu._r.a));    //:M = D&M
}
void MOV_M_DorM()
{
    MMU.ww(cpu._r.a, cpu._r.d | MMU.rw(cpu._r.a));    //:M = D|M
}

void SET_D_zero()
{
    cpu._r.d = 0;    //:D = 0
    cpu._r.st |= F_ZERO;

}
void SET_D_one()
{
    cpu._r.d = 1;    //:D = 1
    cpu._r.st &= ~F_ZERO;
}
void SET_D_negOne()
{
    cpu._r.d = -1;    //:D = -1
    cpu._r.st |= F_SIGN;
}
void IDT_D()
{
    cpu._r.d = cpu._r.d;    //:D = D
}
void MOV_D_A()
{
    cpu._r.d = cpu._r.a;    //:D = A
    DBGREG("LOADING D: cpu._r.d = %x\n", cpu._r.d);
}
void INV_D()
{
    cpu._r.d = !cpu._r.d;    //:D = !D
}
void MOV_D_notA()
{
    cpu._r.d = !cpu._r.a;    //:D = !A
}
void NEG_D()
{
    cpu._r.d = -cpu._r.d;    //:D = -D
}
void MOV_D_negA()
{
    cpu._r.d = -cpu._r.a;    //:D = -A
}
void INC_D()
{
    cpu._r.d++;    //:D = D + 1
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);

}
void MOV_D_incA()
{
    cpu._r.d = cpu._r.a + 1;    //:D = A + 1
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void DEC_D()
{
    cpu._r.d--;    //:D = D - 1
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_D_decA()
{
    cpu._r.d = cpu._r.a - 1;    //:D = A-1
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void ADD_D_A()
{
    cpu._r.d += cpu._r.a;    //:D = D + A
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void SUB_D_A()
{
    cpu._r.d -= cpu._r.a;    //:D = D - A
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_D_AsubD()
{
    cpu._r.d = cpu._r.a - cpu._r.a;    //:D = A - D
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void AND_D_A()
{
    cpu._r.d &= cpu._r.a;    //:D = D & A
}
void OR_D_A()
{
    cpu._r.d |= cpu._r.a;    //:D = D | A
}
void MOV_D_M()
{
    cpu._r.d  = MMU.rw(cpu._r.a);    //:D = M
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_D_notM()
{
    cpu._r.d = !MMU.rw(cpu._r.a);    //:D = !M
}
void MOV_D_negM()
{
    cpu._r.d = -MMU.rw(cpu._r.a);    //:D = -M
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_D_incM()
{
    cpu._r.d  = MMU.rw(cpu._r.a) + 1;    //:D = M+1
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_D_decM()
{
    cpu._r.d  = MMU.rw(cpu._r.a) - 1;    //:D = M-1
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void ADD_D_M()
{
    cpu._r.d += MMU.rw(cpu._r.a);    //:D = D+M
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void SUB_D_M()
{
    cpu._r.d -= MMU.rw(cpu._r.a);    //:D = D-M
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_D_MsubD()
{
    cpu._r.d  = MMU.rw(cpu._r.a) - cpu._r.d;    //:D = M-D
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void AND_D_M()
{
    cpu._r.d &= MMU.rw(cpu._r.a);    //:D = D&M
}
void OR_D_M()
{
    cpu._r.d |= MMU.rw(cpu._r.a);    //:D = D|M
}

void SET_MD_zero()
{
    MMU.ww(cpu._r.a, 0);    //:MD = 0
    cpu._r.d = 0;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void SET_MD_one()
{
    MMU.ww(cpu._r.a, 1);    //:MD = 1
    cpu._r.d = 1;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void SET_MD_negOne()
{
    MMU.ww(cpu._r.a, -1);    //:MD = -1
    cpu._r.d = -1;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_MD_D()
{
    MMU.ww(cpu._r.a, cpu._r.d) ;    //:MD = D
    cpu._r.d = cpu._r.d;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_MD_A()
{
    MMU.ww(cpu._r.a, cpu._r.a);    //:MD = A
    cpu._r.d = cpu._r.a;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_MD_notD()
{
    MMU.ww(cpu._r.a, !cpu._r.d);    //:MD = !D
    cpu._r.d = !cpu._r.d;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_MD_notA()
{
    MMU.ww(cpu._r.a, !cpu._r.a);    //:MD = !A
    cpu._r.d = !cpu._r.a;
}
void MOV_MD_negD()
{
    MMU.ww(cpu._r.a, -cpu._r.d);    //:MD = -D
    cpu._r.d = -cpu._r.d;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_MD_negA()
{
    MMU.ww(cpu._r.a, -cpu._r.a);    //:MD = -A
    cpu._r.d = -cpu._r.a;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_MD_incD()
{
    MMU.ww(cpu._r.a, cpu._r.d + 1);    //:MD = D + 1
    cpu._r.d++;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_MD_incA()
{
    MMU.ww(cpu._r.a, cpu._r.a + 1);    //:MD = A + 1
    cpu._r.d = cpu._r.a + 1;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_MD_decD()
{
    MMU.ww(cpu._r.a, cpu._r.d - 1);    //:MD = D - 1
    cpu._r.d--;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_MD_decA()
{
    MMU.ww(cpu._r.a, cpu._r.a - 1);    //:MD = A-1
    cpu._r.d = cpu._r.a - 1;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_MD_DaddA()
{
    MMU.ww(cpu._r.a, cpu._r.d + cpu._r.a);    //:MD = D + A
    cpu._r.d += cpu._r.a;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_MD_DsubA()
{
    MMU.ww(cpu._r.a, cpu._r.d - cpu._r.a);    //:MD = D - A
    cpu._r.d -= cpu._r.a;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_MD_AsubD()
{
    MMU.ww(cpu._r.a, cpu._r.a - cpu._r.d);    //:MD = A - D
    cpu._r.d = cpu._r.a - cpu._r.d;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_MD_DandA()
{
    MMU.ww(cpu._r.a, cpu._r.d & cpu._r.a);    //:MD = D & A
    cpu._r.d &= cpu._r.a;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_MD_DorA()
{
    MMU.ww(cpu._r.a, cpu._r.d | cpu._r.a);    //:MD = D | A
    cpu._r.d |= cpu._r.a;
}
void MOV_MD_M()
{
    MMU.ww(cpu._r.a, MMU.rw(cpu._r.a));    //:MD = M
    cpu._r.d  = MMU.rw(cpu._r.a);
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_MD_notM()
{
    cpu._r.d = !MMU.rw(cpu._r.a);    //*:MD = !M
    MMU.ww(cpu._r.a, !MMU.rw(cpu._r.a));
}
void MOV_MD_negM()
{
    cpu._r.d = -MMU.rw(cpu._r.a);    //*:MD = -M
    MMU.ww(cpu._r.a, -MMU.rw(cpu._r.a));
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_MD_incM()
{
    cpu._r.d  = MMU.rw(cpu._r.a) + 1;    //:*MD = M+1
    MMU.ww(cpu._r.a, MMU.rw(cpu._r.a) + 1);
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_MD_decM()
{
    cpu._r.d  = MMU.rw(cpu._r.a) - 1;    //:*MD = M-1
    MMU.ww(cpu._r.a, MMU.rw(cpu._r.a) - 1);
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_MD_DaddM()
{
    int out = cpu._r.d + MMU.rw(cpu._r.a);    //*:MD = D+M
    MMU.ww(cpu._r.a, out);
    cpu._r.d = out;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_MD_DsubM()
{
    int out = cpu._r.d - MMU.rw(cpu._r.a);    //*:MD = D-M
    MMU.ww(cpu._r.a, out);
    cpu._r.d = out;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_MD_MsubD()
{
    int out  = MMU.rw(cpu._r.a) - cpu._r.d;    //*:MD = M-D
    MMU.ww(cpu._r.a, out);
    cpu._r.d = out;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_MD_DandM()
{
    int out = cpu._r.d & MMU.rw(cpu._r.a);    //:*MD = D&M
    MMU.ww(cpu._r.a, out);
    cpu._r.d = out;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_MD_DorM()
{
    int out  = cpu._r.d | MMU.rw(cpu._r.a);    //:MD = D|M
    MMU.ww(cpu._r.a, out);
    cpu._r.d = out;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}

void SET_A_zero()
{
    cpu._r.a = 0;    //:A = 0
}
void SET_A_one()
{
    cpu._r.a = 1;    //:A = 1
}
void SET_A_negOne()
{
    cpu._r.a = -1;    //:A = -1
}
void MOV_A_D()
{
    cpu._r.a = cpu._r.d;    //:A = D
}
void IDT_A()
{
    cpu._r.a = cpu._r.a;    //:A = A
}
void MOV_A_notD()
{
    cpu._r.a = !cpu._r.d;    //:A = !D
}
void NOT_A()
{
    cpu._r.a = !cpu._r.a;    //:A = !A
}
void MOV_A_negD()
{
    cpu._r.a = -cpu._r.d;    //:A = -D
}
void NEG_A()
{
    cpu._r.a = -cpu._r.a;    //:A = -A
}
void MOV_A_incD()
{
    cpu._r.a = cpu._r.d + 1;    //:A = D + 1
}
void INC_A()
{
    cpu._r.a += 1;    //:A = A + 1
}
void MOV_A_decD()
{
    cpu._r.a = cpu._r.d - 1;    //:A = D - 1
}
void DEC_A()
{
    cpu._r.a--;    //:A = A-1
}
void MOV_A_DaddA()
{
    cpu._r.a = cpu._r.d + cpu._r.a;    //:A = D + A
}
void MOV_A_DsubA()
{
    cpu._r.a = cpu._r.d - cpu._r.a;    //:A = D - A
}
void SUB_A_D()
{
    cpu._r.a -= cpu._r.d;    //:A = A - D
}
void MOV_A_DandA()
{
    cpu._r.a = cpu._r.d & cpu._r.a;    //:A = D & A
}
void MOV_A_DorA()
{
    cpu._r.a = cpu._r.d | cpu._r.a;    //:A = D | A
}
void MOV_A_M()
{
    cpu._r.a  = MMU.rw(cpu._r.a);    //:A = M
}
void MOV_A_notM()
{
    cpu._r.a = !MMU.rw(cpu._r.a);    //:A = !M
}
void MOV_A_negM()
{
    cpu._r.a = -MMU.rw(cpu._r.a);    //:A = -M
}
void MOV_A_incM()
{
    cpu._r.a  = MMU.rw(cpu._r.a) + 1;    //:A = M+1
}
void MOV_A_decM()
{
    cpu._r.a  = MMU.rw(cpu._r.a) - 1;    //:A = M-1
}
void MOV_A_DaddM()
{
    cpu._r.a = cpu._r.d + MMU.rw(cpu._r.a);    //:A = D+M
}
void MOV_A_DsubM()
{
    cpu._r.a = cpu._r.d - MMU.rw(cpu._r.a);    //:A = D-M
}
void MOV_A_MsuD()
{
    cpu._r.a  = MMU.rw(cpu._r.a) - cpu._r.d;    //:A = M-D
}
void MOV_A_DandM()
{
    cpu._r.a = cpu._r.d & MMU.rw(cpu._r.a);    //:A = D&M
}
void MOV_A_DorM()
{
    cpu._r.a = cpu._r.d | MMU.rw(cpu._r.a);    //:A = D|M
}

void SET_AM_zero()
{
    MMU.ww(cpu._r.a, 0);
    cpu._r.a = 0;    //:AM = 0

}
void SET_AM_one()
{
    MMU.ww(cpu._r.a, 1);
    cpu._r.a = 1;    //:AM = 1

}
void SET_AM_negOne()
{
    MMU.ww(cpu._r.a, -1);
    cpu._r.a = -1;    //:AM = -1

}
void MOV_AM_D()
{
    MMU.ww(cpu._r.a, cpu._r.d);
    cpu._r.a = cpu._r.d;    //:AM = D

}
void MOV_AM_A()
{
    MMU.ww(cpu._r.a, MMU.rw(cpu._r.a));
    cpu._r.a = cpu._r.a;    //:AM = A

}
void MOV_AM_notD()
{
    MMU.ww(cpu._r.a, !cpu._r.d);
    cpu._r.a = !cpu._r.d;    //:AM = !D

}
void MOV_AM_notA()
{
    cpu._r.a  = !cpu._r.a;
    MMU.ww(cpu._r.a, !cpu._r.a);    //*:AM = !A

}
void MOV_AM_negD()
{
    MMU.ww(cpu._r.a, -cpu._r.d);
    cpu._r.a = -cpu._r.d;    //:AM = -D

}
void MOV_AM_negA()
{
    MMU.ww(cpu._r.a, -cpu._r.a);     //*:AM = -A
    cpu._r.a = -cpu._r.a;
}
void MOV_AM_incD()
{
    MMU.ww(cpu._r.a, cpu._r.d + 1);
    cpu._r.a = cpu._r.d + 1;    //:AM = D + 1

}
void MOV_AM_incA()
{
    MMU.ww(cpu._r.a, cpu._r.a + 1);    //*:AM = A + 1
    cpu._r.a = cpu._r.a + 1;


}
void MOV_AM_decD()
{
    MMU.ww(cpu._r.a, cpu._r.d - 1);
    cpu._r.a = cpu._r.d - 1;    //:AM = D - 1

}
void MOV_AM_decA()
{
    MMU.ww(cpu._r.a, cpu._r.a - 1);    //*:AM = A-1
    cpu._r.a = cpu._r.a - 1;
}
void MOV_AM_DaddA()
{
    MMU.ww(cpu._r.a, cpu._r.d + cpu._r.a);    //*:AM = D + A
    cpu._r.a = cpu._r.d + cpu._r.a;
}
void MOV_AM_DsubA()
{
    MMU.ww(cpu._r.a, cpu._r.d - cpu._r.a);    //*:AM = D - A
    cpu._r.a = cpu._r.d - cpu._r.a;
}
void MOV_AM_AsubD()
{
    MMU.ww(cpu._r.a, cpu._r.a - cpu._r.d);    //*:AM = A - D
    cpu._r.a = cpu._r.a - cpu._r.d;
}
void MOV_AM_DandA()
{
    MMU.ww(cpu._r.a, cpu._r.d & cpu._r.a);    //*:AM = D & A
    cpu._r.a = cpu._r.d & cpu._r.a;
}
void MOV_AM_DorA()
{
    MMU.ww(cpu._r.a, cpu._r.d | cpu._r.a);     //*:AM = D | A
    cpu._r.a = cpu._r.d | cpu._r.a;
}
void MOV_AM_M()
{
    int out = MMU.rw(cpu._r.a);
    MMU.ww(cpu._r.a, out);
    cpu._r.a  = out;    //:AM = M
}
void MOV_AM_notM()
{
    int out = !MMU.rw(cpu._r.a);
    MMU.ww(cpu._r.a, out);
    cpu._r.a = out;    //:AM = !M

}
void MOV_AM_negM()
{
    int out = -MMU.rw(cpu._r.a);
    MMU.ww(cpu._r.a, out);
    cpu._r.a = out;    //:AM = -M
}
void MOV_AM_incM()
{
    int out = MMU.rw(cpu._r.a) + 1;
    MMU.ww(cpu._r.a, out);
    cpu._r.a  = out;    //:AM = M+1
}
void MOV_AM_decM()
{
    int out = MMU.rw(cpu._r.a) - 1;
    MMU.ww(cpu._r.a, out);
    cpu._r.a  = out;    //:AM = M-1
}
void MOV_AM_DaddM()
{
    int out = cpu._r.d + MMU.rw(cpu._r.a);
    cpu._r.a = out;  //:AM = D+M
    MMU.ww(cpu._r.a, out);
}
void MOV_AM_DnegM()
{
    int out =  cpu._r.d - MMU.rw(cpu._r.a);
    cpu._r.a = out;  //:AM = D-M
    MMU.ww(cpu._r.a, out);
}
void MOV_AM_MsubD()
{
    int out = MMU.rw(cpu._r.a) - cpu._r.d;
    cpu._r.a  = out;    //:AM = M-D
    MMU.ww(cpu._r.a, out);
}
void MOV_AM_DandM()
{
    int out = cpu._r.d & MMU.rw(cpu._r.a);
    cpu._r.a = out;    //:AM = D&M
    MMU.ww(cpu._r.a, out);
}
void MOV_AM_DorM()
{
    int out = cpu._r.d | MMU.rw(cpu._r.a);
    cpu._r.a = out;    //:AM = D|M
    MMU.ww(cpu._r.a, out);
}

void SET_AD_zero()
{
    cpu._r.a = 0;    //:AD = 0
    cpu._r.d = 0;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void SET_AD_one()
{
    cpu._r.a = 1;    //:AD = 1
    cpu._r.d = 1;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void SET_AD_negOne()
{
    cpu._r.a = -1;    //:AD = -1
    cpu._r.d = 1;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_AD_D()
{
    cpu._r.a = cpu._r.d;    //:AD = D
    cpu._r.d = cpu._r.d;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_AD_A()
{
    cpu._r.a = cpu._r.a;    //:AD = A
    cpu._r.d = cpu._r.d;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_AD_notD()
{
    cpu._r.d = !cpu._r.d;    //*:AD = !D
    cpu._r.a = !cpu._r.d;

}
void MOV_AD_notA()
{
    cpu._r.d = !cpu._r.a;     //*:AD = !A
    cpu._r.a = !cpu._r.a;
}
void MOV_AD_negD()
{
    cpu._r.a = -cpu._r.d;    //:AD = -D
    cpu._r.d = -cpu._r.d;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_AD_negA()
{
    cpu._r.d = -cpu._r.a;    //*:AD = -A
    cpu._r.a = -cpu._r.a;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_AD_incD()
{
    cpu._r.a = cpu._r.d + 1;    //:AD = D + 1
    cpu._r.d = cpu._r.d + 1;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_AD_incA()
{
    cpu._r.d = cpu._r.a + 1;    //*:AD = A + 1
    cpu._r.a = cpu._r.a + 1;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_AD_decD()
{
    cpu._r.a = cpu._r.d - 1;    //:AD = D - 1
    cpu._r.d = cpu._r.d - 1;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_AD_decA()
{
    cpu._r.d = cpu._r.a - 1;    //*:AD = A-1
    cpu._r.a = cpu._r.a - 1;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_AD_DaddA()
{
    int out = cpu._r.d + cpu._r.a;    //*:AD = D + A
    cpu._r.a = cpu._r.d = out;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_AD_DsubA()
{
    int out = cpu._r.d - cpu._r.a;    //*:AD = D - A
    cpu._r.a = cpu._r.d = out;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_AD_AsubD()
{
    int out = cpu._r.a - cpu._r.d;    //*:AD = A - D
    cpu._r.a = cpu._r.d = out;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_AD_DandA()
{
    int out = cpu._r.d & cpu._r.a;    //*:AD = D & A
    cpu._r.a = cpu._r.d = out;

}
void MOV_AD_DorA()
{
    int out =  cpu._r.d | cpu._r.a;    //:AD = D | A
    cpu._r.a = cpu._r.d  = out;
}
void MOV_AD_M()
{
    cpu._r.a = MMU.rw(cpu._r.a);    //:AD = M
    cpu._r.d  = MMU.rw(cpu._r.a);
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_AD_notM()
{
    cpu._r.a = !MMU.rw(cpu._r.a);    //:AD = !M
    cpu._r.d = !MMU.rw(cpu._r.a);
}
void MOV_AD_negM()
{
    cpu._r.a = -MMU.rw(cpu._r.a);    //:AD = -M
    cpu._r.d = -MMU.rw(cpu._r.a);
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_AD_incM()
{
    cpu._r.a  = MMU.rw(cpu._r.a) + 1;    //:AD = M+1
    cpu._r.d  = MMU.rw(cpu._r.a) + 1;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_AD_decM()
{
    cpu._r.a  = MMU.rw(cpu._r.a) - 1;    //:AD = M-1
    cpu._r.d  = MMU.rw(cpu._r.a) - 1;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_AD_DaddM()
{
    cpu._r.a = cpu._r.d + MMU.rw(cpu._r.a);    //:AD = D+M
    cpu._r.d = cpu._r.d + MMU.rw(cpu._r.a);
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_AD_DsubM()
{
    cpu._r.a = cpu._r.d - MMU.rw(cpu._r.a);    //:AD = D-M
    cpu._r.d = cpu._r.d - MMU.rw(cpu._r.a);
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_AD_MsubD()
{
    cpu._r.a  = MMU.rw(cpu._r.a) - cpu._r.d;    //:AD = M-D
    cpu._r.d  = MMU.rw(cpu._r.a) - cpu._r.d;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_AD_DandM()
{
    cpu._r.a = cpu._r.d & MMU.rw(cpu._r.a);    //:AD = D&M
    cpu._r.d = cpu._r.d & MMU.rw(cpu._r.a);
}
void MOV_AD_DorM()
{
    cpu._r.a = cpu._r.a | MMU.rw(cpu._r.a);    //:AD = D|M
    cpu._r.d = cpu._r.d | MMU.rw(cpu._r.a);
}

void SET_AMD_zero()
{
    int out = 0;    //:AMD = 0
    MMU.ww(cpu._r.a, out);
    cpu._r.d = out;
    cpu._r.a = out;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void SET_AMD_one()
{
    int out = 1;    //:AMD = 1
    MMU.ww(cpu._r.a, out);
    cpu._r.d = out;
    cpu._r.a = out;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void SET_AMD_negOne()
{
    int out = -1;    //:AMD = -1
    MMU.ww(cpu._r.a, out);
    cpu._r.d = out;
    cpu._r.a = out;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_AMD_D()
{
    int out = cpu._r.d;    //:AMD = D
    MMU.ww(cpu._r.a, out);
    cpu._r.d = out;
    cpu._r.a = out;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_AMD_A()
{
    int out = cpu._r.a;    //:AMD = A
    MMU.ww(cpu._r.a, out);
    cpu._r.d = out;
    cpu._r.a = out;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_AMD_notD()
{
    int out = !cpu._r.d;    //:AMD = !D
    MMU.ww(cpu._r.a, out);
    cpu._r.d = out;
    cpu._r.a = out;
}
void MOV_AMD_notA()
{
    int out = !cpu._r.a;    //:AMD = !A
    MMU.ww(cpu._r.a, out);
    cpu._r.d = out;
    cpu._r.a = out;
}
void MOV_AMD_negD()
{
    int out = -cpu._r.d;    //:AMD = -D
    MMU.ww(cpu._r.a, out);
    cpu._r.d = out;
    cpu._r.a = out;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_AMD_negA()
{
    int out = -cpu._r.a;    //:AMD = -A
    MMU.ww(cpu._r.a, out);
    cpu._r.d = out;
    cpu._r.a = out;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_AMD_incD()
{
    int out = cpu._r.d + 1;    //:AMD = D + 1
    MMU.ww(cpu._r.a, out);
    cpu._r.d = out;
    cpu._r.a = out;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_AMD_incA()
{
    int out = cpu._r.a + 1;    //:AMD = A + 1
    MMU.ww(cpu._r.a, out);
    cpu._r.d = out;
    cpu._r.a = out;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_AMD_decD()
{
    int out = cpu._r.d - 1;    //:AMD = D - 1
    MMU.ww(cpu._r.a, out);
    cpu._r.d = out;
    cpu._r.a = out;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_AMD_decA()
{
    int out = cpu._r.a - 1;    //:AMD = A-1
    MMU.ww(cpu._r.a, out);
    cpu._r.d = out;
    cpu._r.a = out;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_AMD_DaddA()
{
    int out = cpu._r.d + cpu._r.a;    //:AMD = D + A
    MMU.ww(cpu._r.a, out);
    cpu._r.d = out;
    cpu._r.a = out;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_AMD_DsubA()
{
    int out = cpu._r.d - cpu._r.a;    //:AMD = D - A
    MMU.ww(cpu._r.a, out);
    cpu._r.d = out;
    cpu._r.a = out;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_AMD_AsubD()
{
    int out = cpu._r.a - cpu._r.d;    //:AMD = A - D
    MMU.ww(cpu._r.a, out);
    cpu._r.d = out;
    cpu._r.a = out;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_AMD_DandA()
{
    int out = cpu._r.d & cpu._r.a;    //:AMD = D & A
    MMU.ww(cpu._r.a, out);
    cpu._r.d = out;
    cpu._r.a = out;

}
void MOV_AMD_DorA()
{
    int out =  cpu._r.d | cpu._r.a;    //:AMD = D | A
    MMU.ww(cpu._r.a, out);
    cpu._r.d = out;
    cpu._r.a = out;
}
void MOV_AMD_M()
{
    int out  = MMU.rw(cpu._r.a);    //:AMD = M
    MMU.ww(cpu._r.a, out);
    cpu._r.d = out;
    cpu._r.a = out;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_AMD_notM()
{
    int out = !MMU.rw(cpu._r.a);    //:AMD = !M
    MMU.ww(cpu._r.a, out);
    cpu._r.d = out;
    cpu._r.a = out;
}
void MOV_AMD_negM()
{
    int out = -MMU.rw(cpu._r.a);    //:AMD = -M
    MMU.ww(cpu._r.a, out);
    cpu._r.d = out;
    cpu._r.a = out;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_AMD_incM()
{
    int out  = MMU.rw(cpu._r.a) + 1;    //:AMD = M+1
    MMU.ww(cpu._r.a, out);
    cpu._r.d = out;
    cpu._r.a = out;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_AMD_decM()
{
    int out  = MMU.rw(cpu._r.a) - 1;    //:AMD = M-1
    MMU.ww(cpu._r.a, out);
    cpu._r.d = out;
    cpu._r.a = out;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_AMD_DaddM()
{
    int out = cpu._r.d + MMU.rw(cpu._r.a);    //:AMD = D+M
    MMU.ww(cpu._r.a, out);
    cpu._r.d = out;
    cpu._r.a = out;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_AMD_DsubM()
{
    int out = cpu._r.d - MMU.rw(cpu._r.a);    //:AMD = D-M
    MMU.ww(cpu._r.a, out);
    cpu._r.d = out;
    cpu._r.a = out;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_AMD_MsubD()
{
    int out  = MMU.rw(cpu._r.a) - cpu._r.d;    //:AMD = M-D
    MMU.ww(cpu._r.a, out);
    cpu._r.d = out;
    cpu._r.a = out;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_AMD_DandM()
{
    int out = cpu._r.d & MMU.rw(cpu._r.a);    //:AMD = D&M
    MMU.ww(cpu._r.a, out);
    cpu._r.d = out;
    cpu._r.a = out;
    (cpu._r.d == 0) ? (cpu._r.st |= F_ZERO) : (cpu._r.st &= ~F_ZERO);
    (cpu._r.d < 0) ? (cpu._r.st |= F_SIGN) : (cpu._r.st &= ~F_SIGN);
}
void MOV_AMD_DorM()
{
    int out = cpu._r.d | MMU.rw(cpu._r.a);     //:AMD = D|M
    MMU.ww(cpu._r.a, out);
    cpu._r.d = out;
    cpu._r.a = out;
}

/*
   Example with function pointers where ttt is a struct with an instantiated object
   void (*tempArr[])() = {&fun0, &fun1};
   memcpy(ttt.fptr, tempArr, sizeof(tempArr));
   ttt.a = 6;
   printf("---------- calling function 0.... ----------\n");
   (*ttt.fptr[0])();
   printf("---------- calling function 1.... ----------\n");
   (*ttt.fptr[1])();
*/




/*
  Reset everything back to pristine state

*/
void resetCPU(void)
{
    DBG("Reset not properly implemented yet!!\n");
    memset(&cpu._r, 0x00, sizeof(cpu._r));
    cpu._clock = 0;
}



/* Fill in the cpu struct with the relevent functions, etc. */
void initCPU()
{

    memset(&cpu._r, 0x00, sizeof(cpu._r));    // Clear registers

    cpu._clock = 0;                        // Zero timer
    cpu.reset = resetCPU;                  // Set up reset function
    cpu.execute = execute;                 // Set up execute function

    /* Pull in list of functions/operations */
    memcpy(cpu.map, opList, sizeof(cpu.map));

}


/*
   Actually use the CPU by executing the current instruction
   as pointed to by the program counter;
   increment the pc also;

   note: there may be some issue with pre or post incremement
   when defining the "next instruction"

   Assume we have cpu global

*/
void execute(void)
{
    unsigned char instructionShift = 15;  // location of bit that determines if A or C instruction
    unsigned char jumpBits = 0; // What the jump instruction is
    unsigned int comp; // Value of the compare register
    unsigned int destCode; // Which register are we writing to
    destCode = 0;

    /* The entire opcode should map to a function */

    /* Get the instruction from the ROM */
    unsigned int opcode = MMU.rw(cpu._r.pc);



    /* need to decode A vs C instructions...not sure if this is the best place
       TODO: write an Instruction Decoder function
    */
    printf("Current Opcode %x\n", opcode);
    if ( opcode & (1 << instructionShift) )
    {
        /* if bit15 is 1 then we have a C instruction */
        DBG("------->C INST opcode before swizzle %x\n",opcode);
        opcode &= 0x1FFF; //mask to 13 bits
        //DBG("---opcode after mask to 13 %x\n",opcode);
        jumpBits = opcode & 0x07; // get jump bits for later
        opcode = (opcode >> 3) & 0x3FF; // cut jump bits - should now be down to 10 bits
        //destBits = opcode & 0x07; // get destination bits for later
        //opcode = (opcode >> 3) & 0x3FF; // cut destination bits - should now be down to 7 bits
        //DBG("------!!>opcode after shift3 %x\n",opcode);
        /* Need to add offset as not every opcode is used */
        //opcode -= OPCODE_OFFSET;
        // DBG("------->opcode after shift3 %x\n",opcode);
    }
    else
    {
        opcode = 0x00;  // call A load instruction
        jumpBits = 0x00;
    }
    /* ************ACTUALLY CALCULATE OUTPUT BY RUNNING CPU  ************** */

    printf("PC: %d\n",cpu._r.pc/2);
    DBG("****************executing instruction\n");
    //DBG("SIZEOF CPU MAP %d, contents of [0x300] %x, of [0x8] %x [0xA] %x\n",sizeof(cpu.map), cpu.map[0x300], cpu.map[0x8], cpu.map[0xA]);



    if (jumpBits)
    {
        /* Get the destination bits so we can determine
           the jump
        */

        destCode = (opcode &= 0x3F8);
        destCode >>= 3;
        printf(":::::::WANTED TO JUMP WITH JUMPBITS %x and DESTCODE %x\n",jumpBits, destCode);
        //system("pause");
        /*
              COMP       NUM           HEX
                 0 	  		10 1010		 2A
                 1			11 1111		 3F
                 D			00 1100		 0C
        */
        switch (destCode)
        {
        case 0x0:
            break;
        case 0x0C:
            comp  = cpu._r.d;
            printf("COMPARE REG IS D\n");
            break;
        case 0x2A:
            comp = 0;
            break;
        case 0x3F:
            comp = 1;
            break;
        default:
            printf("execute: ERROR: INVALID JUMP COMPARE BITS\n");
        }

        //cpu._r.pc += 2; // Update Program Counter here - can be overriden by jump

        /* Now we know which register to look at for the compare, we can see where to jump */
        switch (jumpBits)
        {

        case 0:
            cpu._r.pc += 2; // Update Program Counter here if no jump
            printf("NO JUMP!!!\n");
            break; // no jump
        case 1:
            if (comp > 0)
            {
                cpu._r.pc = 2*cpu._r.a;    // Program Counter set to Address register value for jump
                printf("JUMPING > 0, A is %d\n", cpu._r.a);
            }
            else
            {
                cpu._r.pc += 2;
            }
            break;
        case 2:
            printf("Checking if eq zero jump");
            if (comp == 0)
            {
                cpu._r.pc = 2*cpu._r.a;
                printf("!!JUMPING == 0, A is %d\n", cpu._r.a);
            }
            else
            {
                cpu._r.pc += 2;
            }
            break;
        case 3:
            if (comp >= 0)
            {
                cpu._r.pc = 2*cpu._r.a;
                printf("JUMPING >= 0, A is %d\n", cpu._r.a);
            }
            else
            {
                cpu._r.pc += 2;
            }
            break;
        case 4:
            if (comp < 0)
            {
                cpu._r.pc = 2*cpu._r.a;
                DBG("JUMPING < 0, A is %d\n", cpu._r.a);
            }
            else
            {
                cpu._r.pc += 2;
            }
            break;
        case 5:
            if (comp != 0)
            {
                cpu._r.pc = 2*cpu._r.a;
                DBG("JUMPING != 0, A is %d\n", cpu._r.a);
            }
            else
            {
                cpu._r.pc += 2;
            }
            break;
        case 6:
            if (comp <= 0)
            {
                DBG("JUMPING <= 0, A is %d\n", cpu._r.a);
                cpu._r.pc = 2*cpu._r.a;
            }
            else
            {
                cpu._r.pc += 2;
            }
            break;
        case 7:
            if (1)
            {
                DBG("JUMPING ALWAYS, A is %d\n", cpu._r.a);
                cpu._r.pc = 2*cpu._r.a;    // Always jump
                DBG("NOW PC HAS BEEN SET TO (DEC) %d\n", cpu._r.pc);
            }
            else
            {
                cpu._r.pc += 2;
            }
            break;
        default:
            printf("execute: ERROR: INCORRECT JUMP INSTRUCTION\n");
            break;
        }

    }
    else
    {

        /* Run the mapped function */
        printf("EXECUTING %x\n",opcode);
        (*cpu.map[opcode])();
        DBG("****************DONE*************\n");
        cpu._r.pc += 2; // Update Program Counter here
        /* ******************************************************************** */

    }

    printf("XXXXXXXXXXXXXXXXXXXXXXXXX\n");
    //system("pause");
    //system("cls");
    printf("YYYYYYYYYYYYYYYYYYYYYYYYYY\n");


}


/*********************************************************
 *
 *
 *        MAIN FUNCTION
 *
 *
 *
 *
 ********************************************************/

int main()
{
    int i = 0;
    int rc = 0;

    memset(RAM+0x5000,0xFF,sizeof(RAM)-0x5000);

    printf("!!!!!!!!!Loading Program ROM...\n");
    rc = readHackFile(RAM);
    if (rc < 0)
    {
        printf("ERROR LOADING ROM\n");
        return rc;
    }

    printf("calling CPU init...\n");
    initCPU();      // Set up the CPU
    printf("calling CPU reset...\n");
    cpu.reset();    // Clear all the registers
    printf("calling MMU init...\n");
    initMMU();

    /* Run the CPU! */
    // for(;;)
    printf("Running Main Loop...\n");
    for(i=0; i< NUM_EXEC; i++)
    {
        cpu.execute();

        RAM[KBD] = i%256;

        if (DUMP_SCREEN_MEM)
        {
            rc = screenMemDump("screenDump.txt", RAM, SCREEN, SCREEN_SIZE);
        }
        if (rc < 0)
        {
            return -1;
        }

        printf("-----------------cycle: %d -------------\n",i);
    }

    printf("---------------->cpu._r.d = %d\n",cpu._r.d);

    return 0;

}

/* --------------------------------------------------------------- */



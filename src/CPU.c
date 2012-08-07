// Started 7/9/12 Hack CPU Emulator

// Loading immediate values  - get from the opcode A = MEMORY[pc+1] etc.
// The only way to get values into the system is via the @ A instruction

#define DEBUG 1
#define DEBUG_REGS 1
#define OPCODE_OFFSET 0x0561 // not all opcodes are used so we need an offset into the valid ones

#include <stdio.h>
#include <string.h>

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
unsigned char RAM[0xFFFF];
//char RAM[0xFFFF] = {0};
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
   int
   a,   // address register
   d,   // data register
   sp,  // stack pointer
   pc,  // program counter         i,
   ms,   // time taken for last instruction
   t;   // total time CPU has run
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
   void (*map[512])(void);                    // Array of function pointers to operations - organized by opcode
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
   void (*wb) (int address, char value);    // Write byte
   void (*ww) (int address, int value);     // Write word

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

void writeByte (int address, char value)
{
   int mask = 0xFFFF; // todo: set the correct mask
   address &= mask;

   /* make sure we don't try and write to ROM */
   if (address < ROM_END)
   {
      printf("writeByte ERROR: ATTEMPTING TO WRITE TO READ ONLY ROM\n");
      return;
   }

   if (address > RAM_END)
   {
      printf("writeByte ERROR: ATTEMPTING TO WRITE PAST MAPPED MEMORY RANGE\n");
      return;
   }

   /* Now write the data to the RAM */
   RAM[address] = value;
}

void writeWord (int address, int value)
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
   int combined;
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

void LOAD_A()
{
   cpu._r.a = MMU.rw(cpu._r.pc);    //:A = number
   DBGREG("--------->LOADING regA with %x ************\n",cpu._r.a);
}
void SET_M_zero()
{
   MMU.ww(cpu._r.a, 0);    //:M = 0
}
void SET_M_one()
{
   MMU.ww(cpu._r.a, 1);    //:M = 1
}
void SET_M_negOne()
{
   MMU.ww(cpu._r.a, -1);    //:M = -1
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
}
void SET_D_one()
{
   cpu._r.d = 1;    //:D = 1
}
void SET_D_negOne()
{
   cpu._r.d = -1;    //:D = -1
}
void IDT_D()
{
   cpu._r.d = cpu._r.d;    //:D = D
}
void MOV_D_A()
{
   cpu._r.d = cpu._r.a;    //:D = A
   DBGREG("cpu._r.d = %x\n", cpu._r.d);
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
}
void MOV_D_incA()
{
   cpu._r.d = cpu._r.a + 1;    //:D = A + 1
}
void DEC_D()
{
   cpu._r.d--;    //:D = D - 1
}
void MOV_D_decA()
{
   cpu._r.d = cpu._r.a - 1;    //:D = A-1
}
void ADD_D_A()
{
   cpu._r.d += cpu._r.a;    //:D = D + A
}
void SUB_D_A()
{
   cpu._r.d -= cpu._r.a;    //:D = D - A
}
void MOV_D_AsubD()
{
   cpu._r.d = cpu._r.a - cpu._r.a;    //:D = A - D
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
}
void MOV_D_notM()
{
   cpu._r.d = !MMU.rw(cpu._r.a);    //:D = !M
}
void MOV_D_negM()
{
   cpu._r.d = -MMU.rw(cpu._r.a);    //:D = -M
}
void MOV_D_incM()
{
   cpu._r.d  = MMU.rw(cpu._r.a) + 1;    //:D = M+1
}
void MOV_D_decM()
{
   cpu._r.d  = MMU.rw(cpu._r.a) - 1;    //:D = M-1
}
void ADD_D_M()
{
   cpu._r.d += MMU.rw(cpu._r.a);    //:D = D+M
}
void SUB_D_M()
{
   cpu._r.d -= MMU.rw(cpu._r.a);    //:D = D-M
}
void MOV_D_MsubD()
{
   cpu._r.d  = MMU.rw(cpu._r.a) - cpu._r.d;    //:D = M-D
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
}
void SET_MD_one()
{
   MMU.ww(cpu._r.a, 1);    //:MD = 1
   cpu._r.d = 1;
}
void SET_MD_negOne()
{
   MMU.ww(cpu._r.a, -1);    //:MD = -1
   cpu._r.d = -1;
}
void MOV_MD_D()
{
   MMU.ww(cpu._r.a, cpu._r.d) ;    //:MD = D
   cpu._r.d = cpu._r.d;
}
void MOV_MD_A()
{
   MMU.ww(cpu._r.a, cpu._r.a);    //:MD = A
   cpu._r.d = cpu._r.a;
}
void MOV_MD_notD()
{
   MMU.ww(cpu._r.a, !cpu._r.d);    //:MD = !D
   cpu._r.d != cpu._r.d;
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
}
void MOV_MD_negA()
{
   MMU.ww(cpu._r.a, -cpu._r.a);    //:MD = -A
   cpu._r.d = -cpu._r.a;
}
void MOV_MD_incD()
{
   MMU.ww(cpu._r.a, cpu._r.d + 1);    //:MD = D + 1
   cpu._r.d++;
}
void MOV_MD_incA()
{
   MMU.ww(cpu._r.a, cpu._r.a + 1);    //:MD = A + 1
   cpu._r.d = cpu._r.a + 1;
}
void MOV_MD_decD()
{
   MMU.ww(cpu._r.a, cpu._r.d - 1);    //:MD = D - 1
   cpu._r.d--;
}
void MOV_MD_decA()
{
   MMU.ww(cpu._r.a, cpu._r.a - 1);    //:MD = A-1
   cpu._r.d = cpu._r.a - 1;
}
void MOV_MD_DaddA()
{
   MMU.ww(cpu._r.a, cpu._r.d + cpu._r.a);    //:MD = D + A
   cpu._r.d += cpu._r.a;
}
void MOV_MD_DsubA()
{
   MMU.ww(cpu._r.a, cpu._r.d - cpu._r.a);    //:MD = D - A
   cpu._r.d -= cpu._r.a;
}
void MOV_MD_AsubD()
{
   MMU.ww(cpu._r.a, cpu._r.a - cpu._r.d);    //:MD = A - D
   cpu._r.d = cpu._r.a - cpu._r.d;
}
void MOV_MD_DandA()
{
   MMU.ww(cpu._r.a, cpu._r.d & cpu._r.a);    //:MD = D & A
   cpu._r.d &= cpu._r.a;
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
}
void MOV_MD_incM()
{
   cpu._r.d  = MMU.rw(cpu._r.a) + 1;    //:*MD = M+1
   MMU.ww(cpu._r.a, MMU.rw(cpu._r.a) + 1);
}
void MOV_MD_decM()
{
   cpu._r.d  = MMU.rw(cpu._r.a) - 1;    //:*MD = M-1
   MMU.ww(cpu._r.a, MMU.rw(cpu._r.a) - 1);
}
void MOV_MD_DaddM()
{
   int out = cpu._r.d + MMU.rw(cpu._r.a);    //*:MD = D+M
   MMU.ww(cpu._r.a, out);
   cpu._r.d = out;
}
void MOV_MD_DsubM()
{
   int out = cpu._r.d - MMU.rw(cpu._r.a);    //*:MD = D-M
   MMU.ww(cpu._r.a, out);
   cpu._r.d = out;
}
void MOV_MD_MsubD()
{
   int out  = MMU.rw(cpu._r.a) - cpu._r.d;    //*:MD = M-D
   MMU.ww(cpu._r.a, out);
   cpu._r.d = out;
}
void MOV_MD_DandM()
{
   int out = cpu._r.d & MMU.rw(cpu._r.a);    //:*MD = D&M
   MMU.ww(cpu._r.a, out);
   cpu._r.d = out;
}
void MOV_MD_DorM()
{
   int out  = cpu._r.d | MMU.rw(cpu._r.a);    //:MD = D|M
   MMU.ww(cpu._r.a, out);
   cpu._r.d = out;
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
}
void SET_AD_one()
{
   cpu._r.a = 1;    //:AD = 1
   cpu._r.d = 1;
}
void SET_AD_negOne()
{
   cpu._r.a = -1;    //:AD = -1
   cpu._r.d = 1;
}
void MOV_AD_D()
{
   cpu._r.a = cpu._r.d;    //:AD = D
   cpu._r.d = cpu._r.d;
}
void MOV_AD_A()
{
   cpu._r.a = cpu._r.a;    //:AD = A
   cpu._r.d = cpu._r.d;
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
}
void MOV_AD_negA()
{
   cpu._r.d = -cpu._r.a;    //*:AD = -A
   cpu._r.a = -cpu._r.a;
}
void MOV_AD_incD()
{
   cpu._r.a = cpu._r.d + 1;    //:AD = D + 1
   cpu._r.d = cpu._r.d + 1;
}
void MOV_AD_incA()
{
   cpu._r.d = cpu._r.a + 1;    //*:AD = A + 1
   cpu._r.a = cpu._r.a + 1;
}
void MOV_AD_decD()
{
   cpu._r.a = cpu._r.d - 1;    //:AD = D - 1
   cpu._r.d = cpu._r.d - 1;
}
void MOV_AD_decA()
{
   cpu._r.d = cpu._r.a - 1;    //*:AD = A-1
   cpu._r.a = cpu._r.a - 1;
}
void MOV_AD_DaddA()
{
   int out = cpu._r.d + cpu._r.a;    //*:AD = D + A
   cpu._r.a = cpu._r.d = out;
}
void MOV_AD_DsubA()
{
   int out = cpu._r.d - cpu._r.a;    //*:AD = D - A
   cpu._r.a = cpu._r.d = out;
}
void MOV_AD_AsubD()
{
   int out = cpu._r.a - cpu._r.d;    //*:AD = A - D
   cpu._r.a = cpu._r.d = out;
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
}
void MOV_AD_incM()
{
   cpu._r.a  = MMU.rw(cpu._r.a) + 1;    //:AD = M+1
   cpu._r.d  = MMU.rw(cpu._r.a) + 1;
}
void MOV_AD_decM()
{
   cpu._r.a  = MMU.rw(cpu._r.a) - 1;    //:AD = M-1
   cpu._r.d  = MMU.rw(cpu._r.a) - 1;
}
void MOV_AD_DaddM()
{
   cpu._r.a = cpu._r.d + MMU.rw(cpu._r.a);    //:AD = D+M
   cpu._r.d = cpu._r.d + MMU.rw(cpu._r.a);
}
void MOV_AD_DsubM()
{
   cpu._r.a = cpu._r.d - MMU.rw(cpu._r.a);    //:AD = D-M
   cpu._r.d = cpu._r.d - MMU.rw(cpu._r.a);
}
void MOV_AD_MsubD()
{
   cpu._r.a  = MMU.rw(cpu._r.a) - cpu._r.d;    //:AD = M-D
   cpu._r.d  = MMU.rw(cpu._r.a) - cpu._r.d;
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
}
void SET_AMD_one()
{
   int out = 1;    //:AMD = 1
   MMU.ww(cpu._r.a, out);
   cpu._r.d = out;
   cpu._r.a = out;
}
void SET_AMD_negOne()
{
   int out = -1;    //:AMD = -1
   MMU.ww(cpu._r.a, out);
   cpu._r.d = out;
   cpu._r.a = out;
}
void MOV_AMD_D()
{
   int out = cpu._r.d;    //:AMD = D
   MMU.ww(cpu._r.a, out);
   cpu._r.d = out;
   cpu._r.a = out;
}
void MOV_AMD_A()
{
   int out = cpu._r.a;    //:AMD = A
   MMU.ww(cpu._r.a, out);
   cpu._r.d = out;
   cpu._r.a = out;
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
void MOV_AND_negD()
{
   int out = -cpu._r.d;    //:AMD = -D
   MMU.ww(cpu._r.a, out);
   cpu._r.d = out;
   cpu._r.a = out;
}
void MOV_AMD_negA()
{
   int out = -cpu._r.a;    //:AMD = -A
   MMU.ww(cpu._r.a, out);
   cpu._r.d = out;
   cpu._r.a = out;
}
void MOV_AMD_incD()
{
   int out = cpu._r.d + 1;    //:AMD = D + 1
   MMU.ww(cpu._r.a, out);
   cpu._r.d = out;
   cpu._r.a = out;
}
void MOV_AMD_incA()
{
   int out = cpu._r.a + 1;    //:AMD = A + 1
   MMU.ww(cpu._r.a, out);
   cpu._r.d = out;
   cpu._r.a = out;
}
void MOV_AMD_decD()
{
   int out = cpu._r.d - 1;    //:AMD = D - 1
   MMU.ww(cpu._r.a, out);
   cpu._r.d = out;
   cpu._r.a = out;
}
void MOV_AMD_decA()
{
   int out = cpu._r.a - 1;    //:AMD = A-1
   MMU.ww(cpu._r.a, out);
   cpu._r.d = out;
   cpu._r.a = out;
}
void MOV_AMD_DaddA()
{
   int out = cpu._r.d + cpu._r.a;    //:AMD = D + A
   MMU.ww(cpu._r.a, out);
   cpu._r.d = out;
   cpu._r.a = out;
}
void MOV_AMD_DsubA()
{
   int out = cpu._r.d - cpu._r.a;    //:AMD = D - A
   MMU.ww(cpu._r.a, out);
   cpu._r.d = out;
   cpu._r.a = out;
}
void MOV_AMD_AsubD()
{
   int out = cpu._r.a - cpu._r.d;    //:AMD = A - D
   MMU.ww(cpu._r.a, out);
   cpu._r.d = out;
   cpu._r.a = out;
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
}
void MOV_AMD_incM()
{
   int out  = MMU.rw(cpu._r.a) + 1;    //:AMD = M+1
   MMU.ww(cpu._r.a, out);
   cpu._r.d = out;
   cpu._r.a = out;
}
void MOV_AMD_decM()
{
   int out  = MMU.rw(cpu._r.a) - 1;    //:AMD = M-1
   MMU.ww(cpu._r.a, out);
   cpu._r.d = out;
   cpu._r.a = out;
}
void MOV_AMD_DaddM()
{
   int out = cpu._r.d + MMU.rw(cpu._r.a);    //:AMD = D+M
   MMU.ww(cpu._r.a, out);
   cpu._r.d = out;
   cpu._r.a = out;
}
void MOV_AMD_DsubM()
{
   int out = cpu._r.d - MMU.rw(cpu._r.a);    //:AMD = D-M
   MMU.ww(cpu._r.a, out);
   cpu._r.d = out;
   cpu._r.a = out;
}
void MOV_AMD_MsubD()
{
   int out  = MMU.rw(cpu._r.a) - cpu._r.d;    //:AMD = M-D
   MMU.ww(cpu._r.a, out);
   cpu._r.d = out;
   cpu._r.a = out;
}
void MOV_AMD_DandM()
{
   int out = cpu._r.d & MMU.rw(cpu._r.a);    //:AMD = D&M
   MMU.ww(cpu._r.a, out);
   cpu._r.d = out;
   cpu._r.a = out;
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


/* Note: when we use this we need to memcpy it over to cpu.map */
void (*opList[])() =
{

   LOAD_A,                  //:A = number
   SET_M_zero,              //:M = 0
   SET_M_one,               //:M = 1
   SET_M_negOne,            //:M = -1
   MOV_M_D,                 //:M = D
   MOV_M_A,                 //:M = A
   MOV_M_notD,              //:M = !D
   MOV_M_notA,              //:M = !A
   MOV_M_negD,              //:M = -D
   MOV_M_negA,              //:M = -A
   MOV_M_incD,              //:M = D + 1
   MOV_M_incA,              //:M = A + 1
   MOV_M_decD,              //:M = D - 1
   MOV_M_decA,              //:M = A-1
   MOV_M_DaddA,             //:M = D + A
   MOV_M_DsubA,             //:M = D - A
   MOV_M_AsubD,             //:M = A - D
   MOV_M_DandA,             //:M = D & A
   MOV_M_DorA,              //:M = D | A
   IDT_M,                   //:M = M
   INV_M,                   //:M = !M
   NEG_M,                   //:M = -M
   INC_M,                   //:M = M+1
   DEC_M,                   //:M = M-1
   MOV_M_DaddM,             //:M = D+M
   MOV_M_DsubM,             //:M = D-M
   SUB_M_D,                 //:M = M-D
   MOV_M_DandM,             //:M = D&M
   MOV_M_DorM,              //:M = D|M
   SET_D_zero,              //:D = 0
   SET_D_one,               //:D = 1
   SET_D_negOne,            //:D = -1
   IDT_D,                   //:D = D
   MOV_D_A,                 //:D = A
   INV_D,                   //:D = !D
   MOV_D_notA,              //:D = !A
   NEG_D,                   //:D = -D
   MOV_D_negA,              //:D = -A
   INC_D,                   //:D = D + 1
   MOV_D_incA,              //:D = A + 1
   DEC_D,                   //:D = D - 1
   MOV_D_decA,              //:D = A-1
   ADD_D_A,                 //:D = D + A
   SUB_D_A,                 //:D = D - A
   MOV_D_AsubD,             //:D = A - D
   AND_D_A,                 //:D = D & A
   OR_D_A,                  //:D = D | A
   MOV_D_M,                 //:D = M
   MOV_D_notM,              //:D = !M
   MOV_D_negM,              //:D = -M
   MOV_D_incM,              //:D = M+1
   MOV_D_decM,              //:D = M-1
   ADD_D_M,                 //:D = D+M
   SUB_D_M,                 //:D = D-M
   MOV_D_MsubD,             //:D = M-D
   AND_D_M,                 //:D = D&M
   OR_D_M,                  //:D = D|M
   SET_MD_zero,             //:MD = 0
   SET_MD_one,              //:MD = 1
   SET_MD_negOne,           //:MD = -1
   MOV_MD_D,                //:MD = D
   MOV_MD_A,                //:MD = A
   MOV_MD_notD,             //:MD = !D
   MOV_MD_notA,             //:MD = !A
   MOV_MD_negD,             //:MD = -D
   MOV_MD_negA,             //:MD = -A
   MOV_MD_incD,             //:MD = D + 1
   MOV_MD_incA,             //:MD = A + 1
   MOV_MD_decD,             //:MD = D - 1
   MOV_MD_decA,             //:MD = A-1
   MOV_MD_DaddA,            //:MD = D + A
   MOV_MD_DsubA,            //:MD = D - A
   MOV_MD_AsubD,            //:MD = A - D
   MOV_MD_DandA,            //:MD = D & A
   MOV_MD_DorA,             //:MD = D | A
   MOV_MD_M,                //:MD = M
   MOV_MD_notM,             //:MD = !M
   MOV_MD_negM,             //:MD = -M
   MOV_MD_incM,             //:MD = M+1
   MOV_MD_decM,             //:MD = M-1
   MOV_MD_DaddM,            //:MD = D+M
   MOV_MD_DsubM,            //:MD = D-M
   MOV_MD_MsubD,            //:MD = M-D
   MOV_MD_DandM,            //:MD = D&M
   MOV_MD_DorM,             //:MD = D|M
   SET_A_zero,              //:A = 0
   SET_A_one,               //:A = 1
   SET_A_negOne,            //:A = -1
   MOV_A_D,                 //:A = D
   IDT_A,                   //:A = A
   MOV_A_notD,              //:A = !D
   NOT_A,                   //:A = !A
   MOV_A_negD,              //:A = -D
   NEG_A,                   //:A = -A
   MOV_A_incD,              //:A = D + 1
   INC_A,                   //:A = A + 1
   MOV_A_decD,             //:A = D - 1
   DEC_A,                   //:A = A-1
   MOV_A_DaddA,             //:A = D + A
   MOV_A_DsubA,             //:A = D - A
   SUB_A_D,                 //:A = A - D
   MOV_A_DandA,             //:A = D & A
   MOV_A_DorA,              //:A = D | A
   MOV_A_M,                 //:A = M
   MOV_A_notM,              //:A = !M
   MOV_A_negM,              //:A = -M
   MOV_A_incM,              //:A = M+1
   MOV_A_decM,              //:A = M-1
   MOV_A_DaddM,             //:A = D+M
   MOV_A_DsubM,             //:A = D-M
   MOV_A_MsuD,              //:A = M-D
   MOV_A_DandM,             //:A = D&M
   MOV_A_DorM,              //:A = D|M
   SET_AM_zero,             //:AM = 0
   SET_AM_one,              //:AM = 1
   SET_AM_negOne,           //:AM = -1
   MOV_AM_D,                //:AM = D
   MOV_AM_A,                //:AM = A
   MOV_AM_notD,             //:AM = !D
   MOV_AM_notA,             //:AM = !A
   MOV_AM_negD,             //:AM = -D
   MOV_AM_negA,             //:AM = -A
   MOV_AM_incD,             //:AM = D + 1
   MOV_AM_incA,             //:AM = A + 1
   MOV_AM_decD,             //:AM = D - 1
   MOV_AM_decA,             //:AM = A-1
   MOV_AM_DaddA,            //:AM = D + A
   MOV_AM_DsubA,            //:AM = D - A
   MOV_AM_AsubD,            //:AM = A - D
   MOV_AM_DandA,            //:AM = D & A
   MOV_AM_DorA,             //:AM = D | A
   MOV_AM_M,                //:AM = M
   MOV_AM_notM,             //:AM = !M
   MOV_AM_negM,             //:AM = -M
   MOV_AM_incM,             //:AM = M+1
   MOV_AM_decM,             //:AM = M-1
   MOV_AM_DaddM,            //:AM = D+M
   MOV_AM_DnegM,            //:AM = D-M
   MOV_AM_MsubD,            //:AM = M-D
   MOV_AM_DandM,            //:AM = D&M
   MOV_AM_DorM,             //:AM = D|M
   SET_AD_zero,             //:AD = 0
   SET_AD_one,              //:AD = 1
   SET_AD_negOne,           //:AD = -1
   MOV_AD_D,                //:AD = D
   MOV_AD_A,                //:AD = A
   MOV_AD_notD,             //:AD = !D
   MOV_AD_notA,             //:AD = !A
   MOV_AD_negD,             //:AD = -D
   MOV_AD_negA,             //:AD = -A
   MOV_AD_incD,             //:AD = D + 1
   MOV_AD_incA,             //:AD = A + 1
   MOV_AD_decD,             //:AD = D - 1
   MOV_AD_decA,             //:AD = A-1
   MOV_AD_DaddA,            //:AD = D + A
   MOV_AD_DsubA,            //:AD = D - A
   MOV_AD_AsubD,            //:AD = A - D
   MOV_AD_DandA,            //:AD = D & A
   MOV_AD_DorA,             //:AD = D | A
   MOV_AD_M,                //:AD = M
   MOV_AD_notM,             //:AD = !M
   MOV_AD_negM,             //:AD = -M
   MOV_AD_incM,             //:AD = M+1
   MOV_AD_decM,             //:AD = M-1
   MOV_AD_DaddM,            //:AD = D+M
   MOV_AD_DsubM,            //:AD = D-M
   MOV_AD_MsubD,            //:AD = M-D
   MOV_AD_DandM,            //:AD = D&M
   MOV_AD_DorM,             //:AD = D|M
   SET_AMD_zero,            //:AMD = 0
   SET_AMD_one,             //:AMD = 1
   SET_AMD_negOne,          //:AMD = -1
   MOV_AMD_D,               //:AMD = D
   MOV_AMD_A,               //:AMD = A
   MOV_AMD_notD,            //:AMD = !D
   MOV_AMD_notA,            //:AMD = !A
   MOV_AND_negD,            //:AMD = -D
   MOV_AMD_negA,            //:AMD = -A
   MOV_AMD_incD,            //:AMD = D + 1
   MOV_AMD_incA,            //:AMD = A + 1
   MOV_AMD_decD,            //:AMD = D - 1
   MOV_AMD_decA,            //:AMD = A-1
   MOV_AMD_DaddA,           //:AMD = D + A
   MOV_AMD_DsubA,           //:AMD = D - A
   MOV_AMD_AsubD,           //:AMD = A - D
   MOV_AMD_DandA,           //:AMD = D & A
   MOV_AMD_DorA,            //:AMD = D | A
   MOV_AMD_M,               //:AMD = M
   MOV_AMD_notM,            //:AMD = !M
   MOV_AMD_negM,            //:AMD = -M
   MOV_AMD_incM,            //:AMD = M+1
   MOV_AMD_decM,            //:AMD = M-1
   MOV_AMD_DaddM,           //:AMD = D+M
   MOV_AMD_DsubM,           //:AMD = D-M
   MOV_AMD_MsubD,           //:AMD = M-D
   MOV_AMD_DandM,           //:AMD = D&M
   MOV_AMD_DorM,            //:AMD = D|M

}; //end op code map of function pointers



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
   unsigned int out; // Value of the destination register
   unsigned int destCode; // Which register are we writing to

   /* The entire opcode should map to a function */

   /* Get the instruction from the ROM */
   int opcode = MMU.rw(cpu._r.pc);

   /* need to decode A vs C instructions...not sure if this is the best place
      TODO: write an Instruction Decoder function
   */
   if ( opcode & (1 << instructionShift) )
   {
      /* if bit15 is 1 then we have a C instruction */
      DBG("------->opcode before swizzle %x\n",opcode);
      opcode &= 0x3FFF; //mask to 13 bits
      DBG("---opcode after mask to 13 %x\n",opcode);
      jumpBits = opcode & 0x07; // get jump bits for later
      opcode = opcode >> 3; // cut jump bits
      /* Need to add offset as not every opcode is used */
      opcode -= OPCODE_OFFSET;
      DBG("------->opcode after shift3 %x\n",opcode);
   }
   else
   {
      opcode = 0x00;  // call A load instruction
   }
   /* ************ACTUALLY CALCULATE OUTPUT BY RUNNING CPU  ************** */
   printf("Current Opcode %x\n", (unsigned)opcode);
   printf("PC: %d\n",cpu._r.pc);
   DBG("****************executing instruction\n");
   /* Run the mapped function */
   (*cpu.map[opcode])();
   DBG("****************DONE*************\n");
   /* ******************************************************************** */
   if (jumpBits)
   {
      /* Get the destination bits so we can determine
         the jump
      */
      destCode = opcode &= 0x07;

      switch (destCode)
      {
      case 0x0:
         break;
      case 0x1:
         out  = MMU.rw(cpu._r.a);
         break;
      case 0x2:
         out = cpu._r.d;
         break;
      case 0x3:
         out = cpu._r.d;
         break;
      case 0x4:
         out = cpu._r.a;
         break;
      case 0x5:
         out = cpu._r.a;
         break;
      case 0x6:
         out = cpu._r.a;
         break;
      case 0x7:
         out = cpu._r.a;
         break;
      default:
         printf("execute: ERROR: INVALID JUMP BITS\n");
      }

      /* Now we know which register to look at for the compare, we can see where to jump */
      switch (jumpBits)
      {
      case 0:
         break; // no jump
      case 1:
         if (out > 0)
         {
            cpu._r.pc = cpu._r.a;    // Program Counter set to Address register value for jump
         }
         break;
      case 2:
         if (out == 0)
         {
            cpu._r.pc = cpu._r.a;
         }
         break;
      case 3:
         if (out >= 0)
         {
            cpu._r.pc = cpu._r.a;
         }
         break;
      case 4:
         if (out < 0)
         {
            cpu._r.pc = cpu._r.a;
         }
         break;
      case 5:
         if (out != 0)
         {
            cpu._r.pc = cpu._r.a;
         }
         break;
      case 6:
         if (out <= 0)
         {
            cpu._r.pc = cpu._r.a;
         }
         break;
      case 7:
         if (1)
         {
            cpu._r.pc = cpu._r.a;    // Always jump
         }
         break;
      default:
         printf("execute: ERROR: INCORRECT JUMP INSTRUCTION\n");
         break;
      }

   }
   cpu._r.pc += 2;
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

   printf("Loading Program ROM...\n");
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
   for(i=0; i<6; i++)
   {
      cpu.execute();
   }

   printf("---------------->cpu._r.d = %d\n",cpu._r.d);

   return 0;

}

/* --------------------------------------------------------------- */



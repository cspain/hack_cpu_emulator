include "hack_instruction_set.h"


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
   int a = 0;
   if (a) {
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
void MOV_AMD_negD()
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

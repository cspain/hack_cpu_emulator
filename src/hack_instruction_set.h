#ifndef HACK_INSTRUCTION_SET_H_INCLUDED
#define HACK_INSTRUCTION_SET_H_INCLUDED



void NOP();
void LOAD_A();                   // A = arg from opcode
void MOV_M_DandA();             //:M = D & A
void MOV_M_DaddA();             //:M = D + A
void MOV_M_AsubD();             //:M = A - D
void MOV_M_D();                 //:M = D
void MOV_M_notD();              //:M = !D
void MOV_M_decD();              //:M = D - 1
void MOV_M_negD();              //:M = -D
void MOV_M_DsubA();             //:M = D - A
void MOV_M_DorA();              //:M = D | A
void MOV_M_incD();              //:M = D + 1
void SET_M_zero();              //:M = 0
void MOV_M_A();                 //:M = A
void MOV_M_notA();              //:M = !A
void MOV_M_decA();              //:M = A-1
void MOV_M_negA();              //:M = -A
void MOV_M_incA();              //:M = A + 1
void SET_M_negOne();            //:M = -1
void SET_M_one();               //:M = 1
void MOV_M_DandM();             //:M = D&M
void MOV_M_DaddM();             //:M = D+M
void SUB_M_D();                 //:M = M-D
void MOV_M_DsubM();             //:M = D-M
void MOV_M_DorM();              //:M = D|M
void IDT_M();                   //:M = M
void INV_M();                   //:M = !M
void DEC_M();                   //:M = M-1
void NEG_M();                   //:M = -M
void INC_M();                   //:M = M+1
void AND_D_A();                 //:D = D & A
void ADD_D_A();                 //:D = D + A
void MOV_D_AsubD();             //:D = A - D
void IDT_D();                   //:D = D
void INV_D();                   //:D = !D
void DEC_D();                   //:D = D - 1
void NEG_D();                   //:D = -D
void SUB_D_A();                 //:D = D - A
void OR_D_A();                  //:D = D | A
void INC_D();                   //:D = D + 1
void SET_D_zero();              //:D = 0
void MOV_D_A();                 //:D = A
void MOV_D_notA();              //:D = !A
void MOV_D_decA();              //:D = A-1
void MOV_D_negA();              //:D = -A
void MOV_D_incA();              //:D = A + 1
void SET_D_negOne();            //:D = -1
void SET_D_one();               //:D = 1
void AND_D_M();                 //:D = D&M
void ADD_D_M();                 //:D = D+M
void MOV_D_MsubD();             //:D = M-D
void SUB_D_M();                 //:D = D-M
void OR_D_M();                  //:D = D|M
void MOV_D_M();                 //:D = M
void MOV_D_notM();              //:D = !M
void MOV_D_decM();              //:D = M-1
void MOV_D_negM();              //:D = -M
void MOV_D_incM();              //:D = M+1
void MOV_MD_DandA();            //:MD = D & A
void MOV_MD_DaddA();            //:MD = D + A
void MOV_MD_AsubD();            //:MD = A - D
void MOV_MD_D();                //:MD = D
void MOV_MD_notD();             //:MD = !D
void MOV_MD_decD();             //:MD = D - 1
void MOV_MD_negD();             //:MD = -D
void MOV_MD_DsubA();            //:MD = D - A
void MOV_MD_DorA();             //:MD = D | A
void MOV_MD_incD();             //:MD = D + 1
void SET_MD_zero();             //:MD = 0
void MOV_MD_A();                //:MD = A
void MOV_MD_notA();             //:MD = !A
void MOV_MD_decA();             //:MD = A-1
void MOV_MD_negA();             //:MD = -A
void MOV_MD_incA();             //:MD = A + 1
void SET_MD_negOne();           //:MD = -1
void SET_MD_one();              //:MD = 1
void MOV_MD_DandM();            //:MD = D&M
void MOV_MD_DaddM();            //:MD = D+M
void MOV_MD_MsubD();            //:MD = M-D
void MOV_MD_DsubM();            //:MD = D-M
void MOV_MD_DorM();             //:MD = D|M
void MOV_MD_M();                //:MD = M
void MOV_MD_notM();             //:MD = !M
void MOV_MD_decM();             //:MD = M-1
void MOV_MD_negM();             //:MD = -M
void MOV_MD_incM();             //:MD = M+1
void MOV_A_DandA();             //:A = D & A
void MOV_A_DaddA();             //:A = D + A
void SUB_A_D();                 //:A = A - D
void MOV_A_D();                 //:A = D
void MOV_A_notD();              //:A = !D
void MOV_A_decD();              //:A = D - 1
void MOV_A_negD();              //:A = -D
void MOV_A_DsubA();             //:A = D - A
void MOV_A_DorA();              //:A = D | A
void MOV_A_incD();              //:A = D + 1
void SET_A_zero();              //:A = 0
void IDT_A();                   //:A = A
void NOT_A();                   //:A = !A
void DEC_A();                   //:A = A-1
void NEG_A();                   //:A = -A
void INC_A();                   //:A = A + 1
void SET_A_negOne();            //:A = -1
void SET_A_one();               //:A = 1
void MOV_A_DandM();             //:A = D&M
void MOV_A_DaddM();             //:A = D+M
void MOV_A_MsuD();              //:A = M-D
void MOV_A_DsubM();             //:A = D-M
void MOV_A_DorM();              //:A = D|M
void MOV_A_M();                 //:A = M
void MOV_A_notM();              //:A = !M
void MOV_A_decM();              //:A = M-1
void MOV_A_negM();              //:A = -M
void MOV_A_incM();              //:A = M+1
void MOV_AM_DandA();            //:AM = D & A
void MOV_AM_DaddA();            //:AM = D + A
void MOV_AM_AsubD();            //:AM = A - D
void MOV_AM_D();                //:AM = D
void MOV_AM_notD();             //:AM = !D
void MOV_AM_decD();             //:AM = D - 1
void MOV_AM_negD();             //:AM = -D
void MOV_AM_DsubA();            //:AM = D - A
void MOV_AM_DorA();             //:AM = D | A
void MOV_AM_incD();             //:AM = D + 1
void SET_AM_zero();             //:AM = 0
void MOV_AM_A();                //:AM = A
void MOV_AM_notA();             //:AM = !A
void MOV_AM_decA();             //:AM = A-1
void MOV_AM_negA();             //:AM = -A
void MOV_AM_incA();             //:AM = A + 1
void SET_AM_negOne();           //:AM = -1
void SET_AM_one();              //:AM = 1
void MOV_AM_DandM();            //:AM = D&M
void MOV_AM_DaddM();            //:AM = D+M
void MOV_AM_MsubD();            //:AM = M-D
void MOV_AM_DnegM();            //:AM = D-M
void MOV_AM_DorM();             //:AM = D|M
void MOV_AM_M();                //:AM = M
void MOV_AM_notM();             //:AM = !M
void MOV_AM_decM();             //:AM = M-1
void MOV_AM_negM();             //:AM = -M
void MOV_AM_incM();             //:AM = M+1
void MOV_AD_DandA();            //:AD = D & A
void MOV_AD_DaddA();            //:AD = D + A
void MOV_AD_AsubD();            //:AD = A - D
void MOV_AD_D();                //:AD = D
void MOV_AD_notD();             //:AD = !D
void MOV_AD_decD();             //:AD = D - 1
void MOV_AD_negD();             //:AD = -D
void MOV_AD_DsubA();            //:AD = D - A
void MOV_AD_DorA();             //:AD = D | A
void MOV_AD_incD();             //:AD = D + 1
void SET_AD_zero();             //:AD = 0
void MOV_AD_A();                //:AD = A
void MOV_AD_notA();             //:AD = !A
void MOV_AD_decA();             //:AD = A-1
void MOV_AD_negA();             //:AD = -A
void MOV_AD_incA();             //:AD = A + 1
void SET_AD_negOne();           //:AD = -1
void SET_AD_one();              //:AD = 1
void MOV_AD_DandM();            //:AD = D&M
void MOV_AD_DaddM();            //:AD = D+M
void MOV_AD_MsubD();            //:AD = M-D
void MOV_AD_DsubM();            //:AD = D-M
void MOV_AD_DorM();             //:AD = D|M
void MOV_AD_M();                //:AD = M
void MOV_AD_notM();             //:AD = !M
void MOV_AD_decM();             //:AD = M-1
void MOV_AD_negM();             //:AD = -M
void MOV_AD_incM();             //:AD = M+1
void MOV_AMD_DandA();           //:AMD = D & A
void MOV_AMD_DaddA();           //:AMD = D + A
void MOV_AMD_AsubD();           //:AMD = A - D
void MOV_AMD_D();               //:AMD = D
void MOV_AMD_notD();            //:AMD = !D
void MOV_AMD_decD();            //:AMD = D - 1
void MOV_AMD_negD();            //:AMD = -D
void MOV_AMD_DsubA();           //:AMD = D - A
void MOV_AMD_DorA();            //:AMD = D | A
void MOV_AMD_incD();            //:AMD = D + 1
void SET_AMD_zero();            //:AMD = 0
void MOV_AMD_A();               //:AMD =
void MOV_AMD_notA();            //:AMD = !A
void MOV_AMD_decA();            //:AMD = A-1
void MOV_AMD_negA();            //:AMD = -A
void MOV_AMD_incA();            //:AMD = A + 1
void SET_AMD_negOne();          //:AMD = -1
void SET_AMD_one();             //:AMD = 1
void MOV_AMD_DandM();           //:AMD = D&M
void MOV_AMD_DaddM();           //:AMD = D+M
void MOV_AMD_MsubD();           //:AMD = M-D
void MOV_AMD_DsubM();           //:AMD = D-M
void MOV_AMD_DorM();            //:AMD = D|M
void MOV_AMD_M();               //:AMD = M
void MOV_AMD_notM();            //:AMD = !M
void MOV_AMD_decM();            //:AMD = M-1
void MOV_AMD_negM();            //:AMD = -M
void MOV_AMD_incM();            //:AMD = M+1


void (*opList[])() =
{
    LOAD_A,
    MOV_M_DandA,             //:M = D & A  !!!!!!!!!!!!!!! 1
    AND_D_A,                 //:D = D & A
    MOV_MD_DandA,            //:MD = D & A
    MOV_A_DandA,             //:A = D & A
    MOV_AM_DandA,            //:AM = D & A
    MOV_AD_DandA,            //:AD = D & A
    MOV_AMD_DandA,           //:AMD = D & A
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    MOV_M_DaddA,             //:M = D + A  17
    ADD_D_A,                 //:D = D + A
    MOV_MD_DaddA,            //:MD = D + A
    MOV_A_DaddA,             //:A = D + A
    MOV_AM_DaddA,            //:AM = D + A
    MOV_AD_DaddA,            //:AD = D + A
    MOV_AMD_DaddA,           //:AMD = D + A
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    MOV_M_AsubD,             //:M = A - D 57
    MOV_D_AsubD,             //:D = A - D
    MOV_MD_AsubD,            //:MD = A - D
    SUB_A_D,                 //:A = A - D
    MOV_AM_AsubD,            //:AM = A - D
    MOV_AD_AsubD,            //:AD = A - D
    MOV_AMD_AsubD,           //:AMD = A - D
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    MOV_M_D,                 //:M = D  97
    IDT_D,                   //:D = D
    MOV_MD_D,                //:MD = D
    MOV_A_D,                 //:A = D
    MOV_AM_D,                //:AM = D
    MOV_AD_D,                //:AD = D
    MOV_AMD_D,               //:AMD = D
    NOP,
    MOV_M_notD,              //:M = !D  105
    INV_D,                   //:D = !D
    MOV_MD_notD,             //:MD = !D
    MOV_A_notD,              //:A = !D
    MOV_AM_notD,             //:AM = !D
    MOV_AD_notD,             //:AD = !D
    MOV_AMD_notD,            //:AMD = !D
    NOP,
    MOV_M_decD,              //:M = D - 1  113
    DEC_D,                   //:D = D - 1
    MOV_MD_decD,             //:MD = D - 1
    MOV_A_decD,              //:A = D - 1
    MOV_AM_decD,             //:AM = D - 1
    MOV_AD_decD,             //:AD = D - 1
    MOV_AMD_decD,            //:AMD = D - 1
    NOP,
    MOV_M_negD,              //:M = -D  121
    NEG_D,                   //:D = -D
    MOV_MD_negD,             //:MD = -D
    MOV_A_negD,              //:A = -D
    MOV_AM_negD,             //:AM = -D
    MOV_AD_negD,             //:AD = -D
    MOV_AMD_negD,            //:AMD = -D
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    MOV_M_DsubA,             //:M = D - A  153
    SUB_D_A,                 //:D = D - A
    MOV_MD_DsubA,            //:MD = D - A
    MOV_A_DsubA,             //:A = D - A
    MOV_AM_DsubA,            //:AM = D - A
    MOV_AD_DsubA,            //:AD = D - A
    MOV_AMD_DsubA,           //:AMD = D - A
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    MOV_M_DorA,              //:M = D | A     169
    OR_D_A,                  //:D = D | A
    MOV_MD_DorA,             //:MD = D | A
    MOV_A_DorA,              //:A = D | A
    MOV_AM_DorA,             //:AM = D | A
    MOV_AD_DorA,             //:AD = D | A
    MOV_AMD_DorA,            //:AMD = D | A
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    MOV_M_incD,              //:M = D + 1         249
    INC_D,                   //:D = D + 1
    MOV_MD_incD,             //:MD = D + 1
    MOV_A_incD,              //:A = D + 1
    MOV_AM_incD,             //:AM = D + 1
    MOV_AD_incD,             //:AD = D + 1
    MOV_AMD_incD,            //:AMD = D + 1
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    SET_M_zero,              //:M = 0  337
    SET_D_zero,              //:D = 0
    SET_MD_zero,             //:MD = 0
    SET_A_zero,              //:A = 0
    SET_AM_zero,             //:AM = 0
    SET_AD_zero,             //:AD = 0
    SET_AMD_zero,            //:AMD = 0
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    MOV_M_A,                 //:M = A     385
    MOV_D_A,                 //:D = A
    MOV_MD_A,                //:MD = A
    IDT_A,                   //:A = A
    MOV_AM_A,                //:AM = A
    MOV_AD_A,                //:AD = A
    MOV_AMD_A,               //:AMD = A
    NOP,
    MOV_M_notA,              //:M = !A     393
    MOV_D_notA,              //:D = !A
    MOV_MD_notA,             //:MD = !A
    NOT_A,                   //:A = !A
    MOV_AM_notA,             //:AM = !A
    MOV_AD_notA,             //:AD = !A
    MOV_AMD_notA,            //:AMD = !A
    NOP,
    MOV_M_decA,              //:M = A-1     401
    MOV_D_decA,              //:D = A-1
    MOV_MD_decA,             //:MD = A-1
    DEC_A,                   //:A = A-1
    MOV_AM_decA,             //:AM = A-1
    MOV_AD_decA,             //:AD = A-1
    MOV_AMD_decA,            //:AMD = A-1
    NOP,
    MOV_M_negA,              //:M = -A   409
    MOV_D_negA,              //:D = -A
    MOV_MD_negA,             //:MD = -A
    NEG_A,                   //:A = -A
    MOV_AM_negA,             //:AM = -A
    MOV_AD_negA,             //:AD = -A
    MOV_AMD_negA,            //:AMD = -A
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    MOV_M_incA,              //:M = A + 1   441
    MOV_D_incA,              //:D = A + 1
    MOV_MD_incA,             //:MD = A + 1
    INC_A,                   //:A = A + 1
    MOV_AM_incA,             //:AM = A + 1
    MOV_AD_incA,             //:AD = A + 1
    MOV_AMD_incA,            //:AMD = A + 1
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    SET_M_negOne,            //:M = -1    465
    SET_D_negOne,            //:D = -1
    SET_MD_negOne,           //:MD = -1
    SET_A_negOne,            //:A = -1
    SET_AM_negOne,           //:AM = -1
    SET_AD_negOne,           //:AD = -1
    SET_AMD_negOne,          //:AMD = -1
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    SET_M_one,               //:M = 1    505
    SET_D_one,               //:D = 1
    SET_MD_one,              //:MD = 1
    SET_A_one,               //:A = 1
    SET_AM_one,              //:AM = 1
    SET_AD_one,              //:AD = 1
    SET_AMD_one,             //:AMD = 1
    NOP,
    MOV_M_DandM,             //:M = D&M     513
    AND_D_M,                 //:D = D&M
    MOV_MD_DandM,            //:MD = D&M
    MOV_A_DandM,             //:A = D&M
    MOV_AM_DandM,            //:AM = D&M
    MOV_AD_DandM,            //:AD = D&M
    MOV_AMD_DandM,           //:AMD = D&M
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    MOV_M_DaddM,             //:M = D+M     529
    ADD_D_M,                 //:D = D+M
    MOV_MD_DaddM,            //:MD = D+M
    MOV_A_DaddM,             //:A = D+M
    MOV_AM_DaddM,            //:AM = D+M
    MOV_AD_DaddM,            //:AD = D+M
    MOV_AMD_DaddM,           //:AMD = D+M
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    SUB_M_D,                 //:M = M-D     569
    MOV_D_MsubD,             //:D = M-D
    MOV_MD_MsubD,            //:MD = M-D
    MOV_A_MsuD,              //:A = M-D
    MOV_AM_MsubD,            //:AM = M-D
    MOV_AD_MsubD,            //:AD = M-D
    MOV_AMD_MsubD,           //:AMD = M-D
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    MOV_M_DsubM,             //:M = D-M   665
    SUB_D_M,                 //:D = D - M
    MOV_MD_DsubM,            //:MD = D-M
    MOV_A_DsubM,             //:A = D-M
    MOV_AM_DnegM,            //:AM = D-M
    MOV_AD_DsubM,            //:AD = D-M
    MOV_AMD_DsubM,           //:AMD = D-M
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    MOV_M_DorM,              //:M = D|M   681
    OR_D_M,                  //:D = D | M
    MOV_MD_DorM,             //:MD = D|M
    MOV_A_DorM,              //:A = D|M
    MOV_AM_DorM,             //:AM = D|M
    MOV_AD_DorM,             //:AD = D|M
    MOV_AMD_DorM,            //:AMD = D|M
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    IDT_M,                   //:M = M   897
    MOV_D_M,                 //:D = M
    MOV_MD_M,                //:MD = M
    MOV_A_M,                 //:A = M
    MOV_AM_M,                //:AM = M
    MOV_AD_M,                //:AD = M
    MOV_AMD_M,               //:AMD = M
    NOP,
    INV_M,                   //:M = !M  905
    MOV_D_notM,              //:D = !M
    MOV_MD_notM,             //:MD = !M
    MOV_A_notM,              //:A = !M
    MOV_AM_notM,             //:AM = !M
    MOV_AD_notM,             //:AD = !M
    MOV_AMD_notM,            //:AMD = !M
    NOP,
    DEC_M,                   //:M = M-1    913
    MOV_D_decM,              //:D = M-1
    MOV_MD_decM,             //:MD = M-1
    MOV_A_decM,              //:A = M-1
    MOV_AM_decM,             //:AM = M-1
    MOV_AD_decM,             //:AD = M-1
    MOV_AMD_decM,            //:AMD = M-1
    NOP,
    NEG_M,                   //:M = -M   921
    MOV_D_negM,              //:D = -M
    MOV_MD_negM,             //:MD = -M
    MOV_A_negM,              //:A = -M
    MOV_AM_negM,             //:AM = -M
    MOV_AD_negM,             //:AD = -M
    MOV_AMD_negM,            //:AMD = -M
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    INC_M,                   //:M = M+1   953
    MOV_D_incM,              //:D = M+1
    MOV_MD_incM,             //:MD = M+1
    MOV_A_incM,              //:A = M+1
    MOV_AM_incM,             //:AM = M+1
    MOV_AD_incM,             //:AD = M+1
    MOV_AMD_incM,            //:AMD = M+1
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP,
    NOP
};




#endif // HACK_INSTRUCTION_SET_H_INCLUDED

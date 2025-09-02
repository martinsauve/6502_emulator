#include "../bus.h"
#include "../6502_types.h"
#include "cpu_base.h"

#ifndef CPU_LOAD_STORE_OPS_H
#define CPU_LOAD_STORE_OPS_H
//*******************************************
// LOAD/STORE OPERATIONS
//*******************************************
//LDA
void opLDA_imm(Cpu*, Bus*);
void opLDA_zp(Cpu*, Bus*);
void opLDA_zpX(Cpu*, Bus*);
void opLDA_abs(Cpu*, Bus*);
void opLDA_absX(Cpu*, Bus*);
void opLDA_absY(Cpu*, Bus*);
void opLDA_indX(Cpu*, Bus*);
void opLDA_indY(Cpu*, Bus*);

// LDX
void opLDX_imm(Cpu*, Bus*);
void opLDX_zp(Cpu*, Bus*);
void opLDX_zpY(Cpu*, Bus*);
void opLDX_abs(Cpu*, Bus*);
void opLDX_absY(Cpu*, Bus*);

// LDY
void opLDY_imm(Cpu*, Bus*);
void opLDY_zp(Cpu*, Bus*);
void opLDY_zpX(Cpu*, Bus*);
void opLDY_abs(Cpu*, Bus*);
void opLDY_absX(Cpu*, Bus*);

// STA
void opSTA_zp(Cpu*, Bus*);
void opSTA_zpX(Cpu*, Bus*);
void opSTA_abs(Cpu*, Bus*);
void opSTA_absX(Cpu*, Bus*);
void opSTA_absY(Cpu*, Bus*);
void opSTA_indX(Cpu*, Bus*);
void opSTA_indY(Cpu*, Bus*);

// STX
void opSTX_zp(Cpu*, Bus*);
void opSTX_zpY(Cpu*, Bus*);
void opSTX_abs(Cpu*, Bus*);

// STY
void opSTY_zp(Cpu*, Bus*);
void opSTY_zpX(Cpu*, Bus*);
void opSTY_abs(Cpu*, Bus*);


// transfers

void opTAX(Cpu*, Bus*);

void opPHA(Cpu*, Bus*);
void opPLA(Cpu*, Bus*);


#endif // CPU_LOAD_STORE_OPS_H

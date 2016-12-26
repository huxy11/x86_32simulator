#ifndef __REG_H__
#define __REG_H__

#include "common.h"

enum { R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI };
enum { R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI };
enum { R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH };

typedef struct {
	union {
		union {
			uint32_t _32;
			uint16_t _16;
			uint8_t _8[2];
		}gpr[8];

		struct {
			uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
		};
	};	
	union{
		uint32_t eflags;
		struct {
			unsigned _cf:1;
			unsigned :1;
			unsigned _pf:1;
			unsigned :1;
			unsigned _af:1;
			unsigned :1;
			unsigned _zf:1;
			unsigned _sf:1;
			unsigned _tf:1;
			unsigned _if:1;
			unsigned _df:1;
			unsigned _of:1;
			unsigned _iopl:2;
			unsigned _nt:1;
			unsigned :1;
			unsigned _rf:1;
			unsigned _vm:1;
		};
	};
	swaddr_t eip;
} CPU_state;

extern CPU_state cpu;

static inline int check_reg_index(int index) {
	assert(index >= 0 && index < 8);
	return index;
}

#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)
#define reg_w(index) (cpu.gpr[check_reg_index(index)]._16)
#define reg_b(index) (cpu.gpr[check_reg_index(index) & 0x3]._8[index >> 2])

extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];

#endif

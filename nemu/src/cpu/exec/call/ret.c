#include "cpu/exec/helper.h"
#include "cpu/reg.h"
#include "ret.h"

#define DATA_BYTE 2
#include "ret-template.h"
#undef DATA_BYTE

make_helper(ret) {
	swaddr_t re = swaddr_read(reg_l(R_ESP), 4, 2);
	cpu.eip = re;
	reg_l(R_ESP) += 4;
	print_asm("ret");
//	Log("cpu.eip = %x\n", cpu.eip);
	return 0;
}

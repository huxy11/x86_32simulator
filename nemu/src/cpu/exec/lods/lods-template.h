#include "cpu/exec/template-start.h"

#define instr lods

static void do_execute() {
	Log("0x%x\n", cpu.eip);
	DATA_TYPE src = swaddr_read(REG(R_ESI), DATA_BYTE);
	REG(R_EAX) = src;
	REG(R_ESI) += DATA_BYTE;
	Log("DATA_BYTE = %d\t src = 0x%x\n", DATA_BYTE, src);
	print_asm("lods");
}

make_helper(concat(lods_, SUFFIX)) {
	do_execute();
	return 1;
}

#include "cpu/exec/template-end.h"


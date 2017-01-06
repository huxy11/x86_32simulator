#include "cpu/exec/template-start.h"

#define instr movs

static void do_execute() {
	DATA_TYPE src = swaddr_read(REG(R_ESI), DATA_BYTE);
	swaddr_write(REG(R_EDI), DATA_BYTE, src);
	Log("DATA_BYTE = %d\tsrc = 0x%x\n", DATA_BYTE, src);
	REG(R_EDI) += DATA_BYTE;
	REG(R_ESI) += DATA_BYTE;
}

make_helper(concat(movs_, SUFFIX)) {
	do_execute();
	return 1;
}

#include "cpu/exec/template-end.h"

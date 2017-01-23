#include "cpu/exec/template-start.h"

#define instr movs

static void do_execute() {
	DATA_TYPE src = swaddr_read(reg_l(R_ESI), DATA_BYTE, 3);
	swaddr_write(reg_l(R_EDI), DATA_BYTE, src);
	//Log("DATA_BYTE = %d\tsrc = 0x%x\n", DATA_BYTE, src);
	reg_l(R_EDI) += DATA_BYTE;
	reg_l(R_ESI) += DATA_BYTE;
}

make_helper(concat(movs_, SUFFIX)) {
	do_execute();
	return 1;
}

#include "cpu/exec/template-end.h"

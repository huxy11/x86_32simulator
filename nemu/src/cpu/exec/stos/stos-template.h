#include "cpu/exec/template-start.h"

#define instr stos

static void do_execute() {
	//DATA_TYPE re = swaddr_read(REG(R_EDI), DATA_BYTE);
	swaddr_write(reg_l(R_EDI), DATA_BYTE, REG(R_EAX));
	if (cpu._df == 0)
		reg_l(R_EDI) += DATA_BYTE;
	else
		reg_l(R_EDI) -= DATA_BYTE;
	print_asm("stos");
}

make_helper(concat(stos_, SUFFIX)) {
	do_execute();
	return 1;
}

#include "cpu/exec/template-end.h"

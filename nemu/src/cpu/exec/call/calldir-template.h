#include "cpu/exec/template-start.h"

#define instr calldir

static void do_execute() {
	reg_l(R_ESP) -= 4;
	swaddr_write(reg_l(R_ESP), 4, cpu.eip + 2);
	Warn("Be cautious, as this call instruction may not be 2-byte wide\n");
	cpu.eip = op_src->val - 2;
	print_asm_template1();
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"

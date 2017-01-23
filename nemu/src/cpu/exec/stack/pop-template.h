#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute() {
	uint32_t re = swaddr_read(reg_l(R_ESP), 4, 2);
	OPERAND_W(op_src, re);
	//Log("re = %x\n", re);
	reg_l(R_ESP) += 4;
	print_asm_template1();
}

make_instr_helper(r)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"

#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute() {
	uint32_t re = swaddr_read(REG(R_ESP), DATA_BYTE);
	Log("op_src = %s re = %x", op_src->str, re);
	REG(R_ESP) -= DATA_BYTE;
	op_src->val = 1;
	op_src->reg = 2;
	Log("op_src = %x\n", op_src->val);
	Log("op_src->reg = %x\n", op_src->reg);
	print_asm_template1();
}

make_instr_helper(r)

#include "cpu/exec/template-end.h"

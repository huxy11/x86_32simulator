#include "cpu/exec/template-start.h"

#define instr adc

static void do_execute() {
	DATA_TYPE re = op_src->val + op_dest->val + cpu._cf;
	OPERAND_W(op_dest, re);
	//TODO:set EFLAGS
	print_asm_template2();
}

make_instr_helper(r2rm)

#include "cpu/exec/template-end.h"


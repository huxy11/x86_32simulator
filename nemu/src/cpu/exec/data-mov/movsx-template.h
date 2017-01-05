#include "cpu/exec/template-start.h"

#define instr movsx

static void do_execute() {
	Log("op_src = 0x%x %d\n", op_src->val, op_src->simm);
	DATA_TYPE_S re = (DATA_TYPE_S)op_src->val;
	Log("re = 0x%x %d\n", re, re);
	OPERAND_W(op_dest, re);
	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"

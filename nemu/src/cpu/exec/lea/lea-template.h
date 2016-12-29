#include "cpu/exec/template-start.h"

#define instr lea

static void do_execute() {
	Log("src = %s %x\n", op_src->str, op_src->addr);
	Log("dest = %s %x\n", op_dest->str, op_dest->val);
	OPERAND_W(op_dest,op_src->addr);
	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"

#include "cpu/exec/template-start.h"

#define instr sub

static void do_execute() {
	Log("src = %s   %x\n", op_src->str, op_src->val);
	Log("src = %d \n", op_src->simm);
	Log("dest = %s   %x\n", op_dest->str, op_dest->val);
	Log("dest = %d\n", op_dest->imm);
	print_asm_template2();
}

make_instr_helper(si2rm)

#include "cpu/exec/template-end.h"

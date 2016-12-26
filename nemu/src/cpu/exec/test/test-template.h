#include "cpu/exec/template-start.h"

#define instr test

static void do_execute() {
	Log("src = %x\tstr = %s\n", op_src->val, op_src->str);
	Log("src2 = %x\tstr = %s\n", op_src2->val, op_src2->str);
	Log("dest = %x\tstr = %s\n", op_dest->val, op_src->str);
	print_asm_template2();
}

make_instr_helper(r2rm)

#include "cpu/exec/template-end.h"

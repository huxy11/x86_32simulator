#include "cpu/exec/template-start.h"

#define instr test

statci void do_execute() {
	Log("src = %x\n", op_src->val);
	Log("dest = %x\n", op_dest->val);
	print_asm_template2();
}

make_instr_helper(r2rm)

#include "cpu/exec/template-end.h"

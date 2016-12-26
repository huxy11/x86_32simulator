#include "cpu/exec/template-start.h"

#define instr test

static void do_execute() {
	Log("src = %x\tstr = %s\n", op_src->val, op_src->str);
	Log("src2 = %x\tstr = %s\n", op_src2->val, op_src2->str);
	uint32_t re = op_src->val & op_src2->val;
	Log("re = %x\n", re);
	print_asm_template2src();
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"

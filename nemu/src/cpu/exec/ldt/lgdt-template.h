#include "cpu/exec/template-start.h"

#define instr lgdt

static void do_execute() {
	Log("op->src =%#x\n", op_src->val);
	Log("1");
	print_asm_template1();
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"

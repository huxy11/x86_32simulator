#include "cpu/exec/template-start.h"

#define instr shl

static void do_execute() {

	Log("src %s %d 0x%x\n", op_src->str, op_src->val, op_src->val);
	Log("dest %s %d 0x%x\n", op_dest->str, op_dest->val, op_dest->val);
	print_asm_template2();
}

make_instr_helper(rm_imm)
	
#include "cpu/exec/template-end.h"

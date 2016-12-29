#include "cpu/exec/template-start.h"

#define instr push

static void do_execute() {
	//Log("type = %d\n\tsize = %d\n\tunion = %x\n\tval = %x\n\tstr = %s\n\t", op_src->type, op_src->size, op_src->imm, op_src->val, op_src->str);
	REG(4) -= 4;
	swaddr_write(REG(4), 4, op_src->val);
	Log("op_src = %s %d 0x%x\n", op_src->str, op_src->val, op_src->val);
	print_asm_template1();
}

make_instr_helper(r)
make_instr_helper(rm)
make_instr_helper(i)

#include "cpu/exec/template-end.h"

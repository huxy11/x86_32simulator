#include "cpu/exec/template-start.h"

#define instr push

static void do_execute() {
	//TODO
	Log("type = %d\n\tsize = %d\n\tunion = %x\n\tval = %x\n\tstr = %s\n\t", op_src->type, op_src->size, op_src->imm, op_src->val, op_src->str);
	Log("before ebp = %x\n", REG(4));
	REG(4) += DATA_BYTE;
	Log("after ebp = %x\n", REG(4));
	print_asm_template2();
}

make_instr_helper(r)

#include "cpu/exec/template-end.h"

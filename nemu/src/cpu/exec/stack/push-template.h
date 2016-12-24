#include "cpu/exec/template-start.h"

#define instr push

static void do_execute() {
	//Log("type = %d\n\tsize = %d\n\tunion = %x\n\tval = %x\n\tstr = %s\n\t", op_src->type, op_src->size, op_src->imm, op_src->val, op_src->str);
	REG(4) += DATA_BYTE;
	swaddr_write(REG(4), DATA_BYTE, op_src->val);
	print_asm_template2();
}

make_instr_helper(r)

#include "cpu/exec/template-end.h"

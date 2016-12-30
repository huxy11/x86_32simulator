#include "cpu/exec/template-start.h"

#define instr movzx

static void do_execute() {
	Log("op_src = %s 0x%x\n", op_src->str, op_src->val);
	Log("DATA_BYTE = %d\n", DATA_BYTE);
	DATA_TYPE re = op_src->val;
	OPERAND_W(op_dest, re);
	print_asm_template2();
}

make_instr_helper(rm2r)


#include "cpu/exec/template-end.h"


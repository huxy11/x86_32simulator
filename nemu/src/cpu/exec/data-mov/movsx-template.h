#include "cpu/exec/template-start.h"

#define instr movsx

static void do_execute() {
	DATA_TYPE_S re = op_src->simm;
//	Log("src = %s 0x%x %d\n", op_src->str, op_src->val, op_src->simm);
//	Log("dest = %s 0x %x %d\n", op_dest->str, op_dest->val, op_dest->simm);
	OPERAND_W(op_dest, re);
	print_asm_template2();
}
make_instr_helper(rmb2r)
#if DATA_BYTE == 4
make_instr_helper(rmw2r)
#endif

#include "cpu/exec/template-end.h"

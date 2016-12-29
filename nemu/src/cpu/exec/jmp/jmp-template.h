#include "cpu/exec/template-start.h"

#define instr jmp
static void do_execute() {
	cpu.eip += op_src->val;
	Log("src = %s %d 0x%x\n", op_src->str, op_src->val, op_src->val);
	OPERAND_W(op_dest, op_src->val - 2);
	print_asm_template1();
}

make_instr_helper(i)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"

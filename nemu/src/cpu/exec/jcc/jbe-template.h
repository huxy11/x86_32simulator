#include "cpu/exec/template-start.h"

#define instr jbe

static void do_execute() {
//	Log("op_src->val = %x\top_src->str = %s\n", op_src->val, op_src->str);
	if ((cpu._zf == 1) || (cpu._cf == 1))
		cpu.eip += op_src->val;
	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"

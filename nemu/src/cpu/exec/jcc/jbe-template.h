#include "cpu/exec/template-start.h"

#define instr jbe

static void do_execute() {
	Log("op_src->val = %x\top_src->str = %s\n", op_src->val, op_src->str);
	Log("op_src->simm = %d\n", op_src->simm);
	if ((cpu._zf == 1) || (cpu._cf == 1))
		cpu.eip += op_src->simm;
	print_asm_template1();
}
make_instr_helper(i)

#include "cpu/exec/template-end.h"

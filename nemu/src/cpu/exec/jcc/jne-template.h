#include "cpu/exec/template-start.h"

#define instr jne

static void do_execute() {
	if (!cpu._zf)
		cpu.eip += op_src->val;
	print_asm_template1();
}

make_instr_helper(si)

#include "cpu/exec/template-end.h"
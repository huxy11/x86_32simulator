#include "cpu/exec/template-start.h"

#define instr jg

static void do_execute() {
	if((cpu._zf !=0)&&(cpu._of != cpu._sf))
		cpu.eip += op_src->val;

	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"

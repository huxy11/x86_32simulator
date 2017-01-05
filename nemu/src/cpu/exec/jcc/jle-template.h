#include "cpu/exec/template-start.h"

#define instr jle

static void do_execute() {
	if ((cpu._zf == 1) || (cpu._sf != cpu._of))
		cpu.eip += op_src->simm;
	print_asm_template1();
}
make_instr_helper(i)

#include "cpu/exec/template-end.h"

#include "cpu/exec/template-start.h"

#define instr js

static void do_execute() {
	if (cpu._sf == 1)
		cpu.eip += (DATA_TYPE_S)op_src->simm;
	print_asm_template1();	
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"

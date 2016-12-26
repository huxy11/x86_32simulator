#include "cpu/exec/template-start.h"

#define instr test

statci void do_execute() {
	
	print_asm_template2();
}

make_instr_helper(r2rm)

#include "cpu/exec/template-end.h"

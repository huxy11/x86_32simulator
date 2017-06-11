#include "cpu/exec/template-start.h"

#define instr lea

static void do_execute() {
	OPERAND_W(op_dest,op_src->addr);
	print_asm_template2();
}

make_helper(concat5(instr, _, rm2r, _, SUFFIX)) { 
		return idex(eip, concat4(decode_, rmaddr2r, _, SUFFIX), do_execute); 
	}

#include "cpu/exec/template-end.h"


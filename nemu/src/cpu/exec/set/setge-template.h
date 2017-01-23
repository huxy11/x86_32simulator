#include "cpu/exec/template-start.h"
#define instr setge
static void do_execute()
{
	OPERAND_W(op_src, cpu._sf == cpu._of);
	print_asm_template1();
	Warn("cpu._sf == cpu._of = %d\n", cpu._sf == cpu._of);
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"

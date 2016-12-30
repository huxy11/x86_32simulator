#include "cpu/exec/template-start.h"

#define instr jg

static void do_execute() {
	Log("cpu.eip = 0x%x\n", cpu.eip);
	Log("src-val = 0x%x\n", op_src->val);
	if((cpu._zf ==0)&&(cpu._of == cpu._sf))
		cpu.eip += op_src->val;

	Log("cpu.eip = 0x%x\n", cpu.eip);

	print_asm_template1();
}

make_instr_helper(si)

#include "cpu/exec/template-end.h"

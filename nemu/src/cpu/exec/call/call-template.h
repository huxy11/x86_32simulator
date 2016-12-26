#include "cpu/exec/template-start.h"
#include "cpu/reg.h"
#define instr call

static void do_execute() {
	Log("op_src->val = %x\n", op_src->val);
	Log("cpu.eip = %x\n", cpu.eip);
	Log("esp = %x\n", REG(R_ESP));
	REG(R_ESP) += 4;
	swaddr_write(REG(R_ESP), 4, cpu.eip);
	cpu.eip += op_src->val;
	print_asm_template1();
}

make_instr_helper(i)


#include "cpu/exec/template-end.h"

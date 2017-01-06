#include "cpu/exec/template-start.h"

#define instr ret

static void do_execute() {
	swaddr_t re = swaddr_read(reg_l(R_ESP), 4);
	cpu.eip = re + op_src->val;
	Log("cpu.eip = 0x%x op_src->val = 0x%x\n", cpu.eip, op_src->val);
	reg_l(R_ESP) += 4;
	Log("cpu.eip = 0x%x\n", cpu.eip);
	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"

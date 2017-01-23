#include "cpu/exec/template-start.h"

#define instr ret

static void do_execute() {
//	Log("cpu.eip = 0x%x\n", cpu.eip);
	swaddr_t re = swaddr_read(reg_l(R_ESP), 4, 2);
	cpu.eip = re - 3;
	reg_l(R_ESP) += (4 + op_src->val);
//	Log("cpu.eip = 0x%x\n", cpu.eip);
	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"

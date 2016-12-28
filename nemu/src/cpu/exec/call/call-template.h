#include "cpu/exec/template-start.h"
#include "cpu/reg.h"
#define instr call

static void do_execute() {
	//Log("cpu.eip = %x\n", cpu.eip);
	REG(R_ESP) += 4;
	swaddr_write(REG(R_ESP), 4, cpu.eip + DATA_BYTE + 1);
	cpu.eip += op_src->val;
	//Log("cpu.eip = %x\n", cpu.eip);
	print_asm_template1();
}

make_instr_helper(i)


#include "cpu/exec/template-end.h"

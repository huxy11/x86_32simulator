#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute() {
	uint32_t re = swaddr_read(REG(R_ESP), DATA_BYTE);
	OPERAND_W(op_src, re);
	Log("re = %x\n", re);
	REG(R_ESP) -= DATA_BYTE;
	print_asm_template1();
}

make_instr_helper(r)

#include "cpu/exec/template-end.h"

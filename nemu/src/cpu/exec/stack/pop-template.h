#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute() {
	uint32_t re = swaddr_read(REG(R_ESP), DATA_BYTE);
	Log("op_src = %s re = %x", op_src->str, re);
	REG(R_ESP) -= DATA_BYTE;
}

make_instr_helper(r)

#include "cpu/exec/template-end.h"

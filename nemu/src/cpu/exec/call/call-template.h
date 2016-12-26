#include "cpu/exec/template-start.h"
#include "cpu/reg.h"
#define instr call

static void do_execute() {
	Log("op_src->val = %x\n", op_src->val);
	Log("cpu.eip = %x\n", cpu.eip);
	Log("esp = %x\n", REG(R_ESP));
}

make_instr_helper(i)


#include "cpu/exec/template-end.h"

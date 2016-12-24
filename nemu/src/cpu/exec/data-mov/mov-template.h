#include "cpu/exec/template-start.h"

#define instr mov

static void do_execute() {
	OPERAND_W(op_dest, op_src->val);
	Log("type = %d\n\tsize = %d\n\tunion = %x\n\tval = %x\n\tstr = %s\n\t", op_src->type, op_src->size, op_src->imm, op_src->val, op_src->str);
	print_asm_template2();
}

make_instr_helper(i2r)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

make_helper(concat(mov_a2moffs_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	MEM_W(addr, REG(R_EAX));

//	Log("%s\n", str(mov_a2moffs));
//	Log("mov" str(SUFFIX) " %%%s,0x%x", REG_NAME(R_EAX), addr);

	print_asm("mov" str(SUFFIX) " %%%s,0x%x", REG_NAME(R_EAX), addr);
	return 5;
}

make_helper(concat(mov_moffs2a_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	REG(R_EAX) = MEM_R(addr);

//	Log("%s\n", str(mov_a2moffs));
//	Log("mov" str(SUFFIX) " %%%s,0x%x", REG_NAME(R_EAX), addr);

	print_asm("mov" str(SUFFIX) " 0x%x,%%%s", addr, REG_NAME(R_EAX));
	return 5;
}

#include "cpu/exec/template-end.h"

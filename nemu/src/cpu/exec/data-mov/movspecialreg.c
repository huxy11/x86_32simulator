#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

make_helper(mov_r2cr)
{
	ModR_M op;
	op.val = instr_fetch(eip + 1, 1);
	Assert(op.mod == 3, "op.mod != 3!");
	if (op.reg == 0) {
		cpu.cr0 = reg_l(op.R_M);
		print_asm("movl %%%s, %%cr0", regsl[op.R_M]);
	} else if (op.reg == 3) {
		cpu.cr3 = reg_l(op.R_M);
		print_asm("movl %%%s, %%cr3", regsl[op.R_M]);
	} else 
		panic("target is not cr0 nor cr3!\n");
	return 2;
}

make_helper(mov_cr2r)
{
	ModR_M op;
	op.val = instr_fetch(eip + 1, 1);
	Assert(op.mod == 3, "op.mod != 3!");
	if (op.reg == 0) {
		reg_l(op.R_M) = cpu.cr0;
		print_asm("movl %%cr0, %%%s", regsl[op.R_M]);
	} else if (op.reg == 3) {
		reg_l(op.R_M) = cpu.cr3;
		print_asm("movl %%cr3, %%%s", regsl[op.R_M]);	
	} else
		panic("source is not cr0 nor cr3!\n");
	return 2;
}

make_helper(mov_r2sr)
{
	ModR_M op;
	op.val = instr_fetch(eip + 1, 1);
	Assert(op.mod == 3, "op.mod != 3!");
	cpu.sreg[op.reg] = reg_w(op.R_M);
   	print_asm("mov %%%s, %%%s", regsw[op.R_M], sregs[op.reg]);
	return 2;
}

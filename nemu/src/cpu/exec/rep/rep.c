#include "cpu/exec/helper.h"
#include "cpu/reg.h"
#include "rep.h"
make_helper(exec);

make_helper(rep_my) {
	print_asm("rep_start");
	int instr_len = 1;
	while (reg_l(R_ECX) >0) {
		instr_len = exec(cpu.eip+1);
		reg_l(R_ECX)--;
	}

	print_asm("rep_end");
	return instr_len+1;
}

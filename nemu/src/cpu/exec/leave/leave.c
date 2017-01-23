#include "leave.h"

make_helper(leave) {
	reg_l(R_ESP) = reg_l(R_EBP);
//	Log("esp = 0x%x\n", reg_l(R_ESP));
	reg_l(R_EBP) = swaddr_read(reg_l(R_ESP), 4, 2);
//	Log("esp = 0x%x\n", reg_l(R_EBP));
	reg_l(R_ESP) += 4;
	print_asm("leave");
	return 1;
}

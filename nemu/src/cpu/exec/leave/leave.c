#include "leave.h"

make_helper(leave) {
	reg_l(R_ESP) = reg_l(R_EBP);
	reg_l(R_EBP) = swaddr_read(R_ESP, 4);
	reg_l(R_ESP) += 4;
	print_asm("leave");
	return 1;
}

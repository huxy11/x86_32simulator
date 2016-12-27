#include "ret.h"

make_helper(ret) {
	swaddr_t re = swaddr_read(reg_l(R_ESP), 4);
	cpu.eip = re;
	reg_l(R_ESP) -= 4;
	print_asm_template1();
	Log("cpu.eip = %x\n", cpu.eip);
	return 0;
}

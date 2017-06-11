#include "ljmp.h"

make_helper(ljmp_l)
{
	uint32_t cs_tmp = instr_fetch(eip + 1, 4);
	uint16_t eip_tmp = instr_fetch(eip + 5, 2);
	cpu.eip = cs_tmp + eip_tmp;
	Log("eip = %#-8x\n", cpu.eip);
	print_asm("ljmp %#-4x:%#-8x", eip_tmp, cs_tmp);
	return 0;
}
make_helper(ljmp_w)
{
	panic("ljmp_w to be implemented");
	return 0;
}

make_helper_v(ljmp)



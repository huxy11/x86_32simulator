#include "ljmp.h"

make_helper(ljmp_l)
{
	uint32_t eip_tmp = instr_fetch(eip + 1, 4);
	uint16_t cs_tmp = instr_fetch(eip + 5, 2);
	cpu.cs = cs_tmp;
	cpu.eip = eip_tmp;
	print_asm("ljmp %#-4x:%#-8x", cs_tmp, eip_tmp);
	return 0;
}
make_helper(ljmp_w)
{
	panic("ljmp_w to be implemented");
	return 0;
}

make_helper_v(ljmp)



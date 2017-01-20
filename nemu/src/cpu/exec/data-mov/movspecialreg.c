#include "cpu/exec/helper.h"

make_helper(mov_r2cr)
{
	Log("eip = %#x\n", eip);
	uint8_t op = instr_fetch(eip + 1, 1);
	Log("op = %#x\n", op);
	return 2;
}

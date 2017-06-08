#include "cpu/exec/helper.h"

#if 0
#define DATA_BYTE 4
#include "lgdt-template.h"
#undef DATA_BYTE
#endif

make_helper(lgdt) {
	decode_i_w(eip + 1);
	//Log("op->src = %#x\n", op_src->val);
	cpu.gdtr_lmt = op_src->val;
	decode_i_l(eip + 3);
	//i//ddLog("op->src = %#x\n", op_src->val);
	cpu.gdtr = op_src->val;
	print_asm("lgdt");
	return 6;
}

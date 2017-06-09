#include "cpu/exec/helper.h"

#if 0
#define DATA_BYTE 4
#include "lgdt-template.h"
#undef DATA_BYTE
#endif

make_helper(lgdt) {
//	decode_i_w(eip + 1);
//	Log("op->src = %#x\n", op_src->val);
//	cpu.gdtr_lmt = op_src->val;
	uint32_t len = decode_rm_w(eip + 1);
	//Log("op_src = %#x\n", op_src->val);
	//Log("op_src.addr = %#x\n", op_src->addr);
	uint32_t result = swaddr_read(op_src->addr + 2, 4, 3);
	//Log("result = %#x\n", result);
	cpu.gdtr.lmt = op_src->val;
	cpu.gdtr.base = result;
	print_asm("lgdt");
	return len + 1;
}

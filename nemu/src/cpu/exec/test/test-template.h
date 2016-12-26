#include "cpu/exec/template-start.h"

#define instr test

#if DATA_BYTE == 4
#define MS 0x8000
#elif DATA_BYTE == 2
#define MS 0x80
#endif

static void do_execute() {
	Log("src = %x\tstr = %s\n", op_src->val, op_src->str);
	Log("src2 = %x\tstr = %s\n", op_src2->val, op_src2->str);
	uint32_t tmp = 1,re = op_src->val & op_src2->val;
	uint8_t i;
	Log("re = %x\n", re);
	cpu._of = 0;
	cpu._cf = 0;
	cpu._sf = re & MS;
	if (re == 0)
		cpu._zf = 0;
	else 
		cpu._zf = 1;
	cpu._pf = 0;
	for (i = 0; i < DATA_BYTE * 8; i++) {
		if (tmp & re) {
			Log("%d is even\n", i);
			cpu._pf = ~cpu._pf;
		}
		tmp <<= 1;
	}
	print_asm_template2src();
}

make_instr_helper(rm)

#undef MS
#include "cpu/exec/template-end.h"

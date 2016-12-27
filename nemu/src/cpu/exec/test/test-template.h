#include "cpu/exec/template-start.h"

#define instr test

static void do_execute() {
	uint32_t tmp = 1,re = op_src->val & op_src2->val;
	uint8_t i;
	cpu._of = 0;
	cpu._cf = 0;
	//Log("%x\n", re & MS);
	//cpu._sf = (re & MS) >> (DATA_BYTE * 8 - 1);
	/*
	if ((re & MS) == MS)
		cpu._sf = 1;
	else
		cpu._sf = 0;
	*/
	check_sf(re);

	check_zf(re);
	cpu._pf = 1;
	for (i = 0; i < DATA_BYTE * 8; i++) {
		if (tmp & re) {
			Log("%d is set\n", i);
			cpu._pf = ~cpu._pf;
		}
		tmp <<= 1;
	}
	Log("src = %x\tstr = %s\n", op_src->val, op_src->str);
	Log("src2 = %x\tstr = %s\n", op_src2->val, op_src2->str);
	Log("re = %x\n", re);
	Log("sf = %x\nzf = %x\npf = %x\n", cpu._sf, cpu._zf, cpu._pf);
	print_asm_template2src();
}

make_instr_helper(rm)

//#undef MS
#include "cpu/exec/template-end.h"

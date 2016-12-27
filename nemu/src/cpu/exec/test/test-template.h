#include "cpu/exec/template-start.h"

#define instr test

static void do_execute() {
	uint32_t re = op_src->val & op_src2->val;
//	uint8_t i;
	cpu._of = 0;
	cpu._cf = 0;
	check_sf(re);
	check_zf(re);
	check_pf(re);

	Log("src = %x\tstr = %s\n", op_src->val, op_src->str);
	Log("src2 = %x\tstr = %s\n", op_src2->val, op_src2->str);
	Log("re = %x\n", re);
	Log("sf = %x\nzf = %x\npf = %x\n", cpu._sf, cpu._zf, cpu._pf);
	print_asm_template2src();
}

make_instr_helper(rm)

//#undef MS
#include "cpu/exec/template-end.h"

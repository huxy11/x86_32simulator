#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute() {
	Log("src->val = %x\tsrc->simm = %d\tsrc->str = %s\n", op_src->val, op_src->simm, op_src->str);
	Log("dest->val = %x\t dest->str = %s\n", op_dest->val, op_dest->str);
	//op_src->val = ~op_src->val + 1;
	Log("src->val = %x\tsrc->simm = %d  %x\tsrc->str = %s\n", op_src->val, op_src->simm, op_src->simm, op_src->str);
	uint32_t re = op_dest->val - op_src->val;
	Log("result = %x  %d\n", re, re);
	check_sf(re);
	check_zf(re);
	check_pf(re);
	check_sub_cf(op_src->val, op_dest->val);
	check_sub_of(op_src->val, op_dest->val, re);
	check_sub_af(op_src->val, op_dest->val);
	print_asm_template2();
}
make_instr_helper(si2rm)
make_instr_helper(i2rm)
make_instr_helper(r2rm)

#include "cpu/exec/template-end.h"

#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute() {
	Log("src->val = %x\tsrc->simm = %d\tsrc->str = %s\n", op_src->val, op_src->simm, op_src->str);
	Log("dest->val = %x\t dest->str = %s\n", op_dest->val, op_dest->str);
	uint32_t re = op_dest->val - op_src->simm;
	Log("result = %x\n", re);
	check_sf(re);
	check_zf(re);
	check_pf(re);
	print_asm_template1();
}

make_instr_helper(si2rm)

#include "cpu/exec/template-end.h"

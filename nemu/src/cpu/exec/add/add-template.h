#include "cpu/exec/template-start.h"

#define instr add

static void do_execute() {
	uint32_t re = op_dest->val + op_src->val;
//	Log("re = %d 0x%x\n", re, re);
	check_pf(re);
	check_sf(re);
	check_zf(re);
	check_add_cf(op_src->val, op_dest->val);
	check_add_of(op_src->val, op_dest->val, re);
	check_add_af(op_src->val, op_dest->val);

	OPERAND_W(op_dest, re);
	print_asm_template2();
}

make_instr_helper(r2rm)
#if DATA_BYTE != 1
make_instr_helper(si2rm)
#endif 
make_instr_helper(i2a)
make_instr_helper(rm2r)
make_instr_helper(i2rm)

#include "cpu/exec/template-end.h"

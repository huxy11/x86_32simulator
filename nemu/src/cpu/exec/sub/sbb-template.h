#include "cpu/exec/template-start.h"

#define instr sbb

static void do_execute() {
	op_dest->val -= cpu._cf;
	DATA_TYPE re = op_dest->val - op_src->val;
	OPERAND_W(op_dest, re);
	check_pf(re);
	check_sf(re);
	check_zf(re);
	check_sub_cf(op_src->val, op_dest->val);
	check_sub_of(op_src->val, op_dest->val, re);
	check_sub_af(op_src->val, op_dest->val);

	print_asm_template2();
}
make_instr_helper(i2a)
make_instr_helper(i2rm)
#if DATA_BYTE != 1
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"

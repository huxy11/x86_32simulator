#include "cpu/exec/template-start.h"

#define instr adc

static void do_execute() {
	op_src->val += cpu._cf;
	DATA_TYPE re = op_src->val + op_dest->val;
	OPERAND_W(op_dest, re);
	check_pf(re);
	check_sf(re);
	check_zf(re);
	check_add_of(op_src->val, op_dest->val, re);
	check_add_af(op_src->val, op_dest->val);
	check_add_cf(op_src->val, op_dest->val);
	print_asm_template2();
}

make_instr_helper(r2rm)

#include "cpu/exec/template-end.h"


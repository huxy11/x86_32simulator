#include "cpu/exec/template-start.h"

#define instr inc

static void do_execute () {
	DATA_TYPE result = op_src->val + 1;
	OPERAND_W(op_src, result);

	check_zf(result);
	check_sf(result);
	check_pf(result);
	check_add_of(1, op_src->val, result);
	check_add_cf(1, op_src->val);
	check_add_af(1, op_src->val);
	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"

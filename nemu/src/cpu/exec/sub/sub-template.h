#include "cpu/exec/template-start.h"

#define instr sub

static void do_execute() {
//	Log("src = %s   0x%x\n", op_src->str, op_src->val);
//	Log("src = %d \n", op_src->simm);
//	Log("dest = %s   0x%x\n", op_dest->str, op_dest->val);
//	Log("dest = %d\n", op_dest->imm);
	uint32_t re = op_dest->val - op_src->val;
//	Log("re = %d 0x%x\n", re, re);
	check_pf(re);
	check_sf(re);
	check_zf(re);
	check_sub_cf(op_src->val, op_dest->val);
	check_sub_of(op_src->val, op_dest->val, re);
	check_sub_af(op_src->val, op_dest->val);
	
//	Log("\nof = %d\nsf = %d\nzf = %d\naf =%d\npf = %d\ncf =%d\n",cpu._of, cpu._sf, cpu._zf, cpu._af, cpu._pf, cpu._cf);

	OPERAND_W(op_dest, re);
	print_asm_template2();
}
#if DATA_BYTE != 1
make_instr_helper(si2rm)
#endif
make_instr_helper(rm2r)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(i2a)

#include "cpu/exec/template-end.h"

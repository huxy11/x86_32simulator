#include "cpu/exec/template-start.h"

#define instr call

static void do_execute() {
	Log("op_src->val = %x\n", op_src->val);
		
}

make_instr_helper(i)


#include "cpu/exec/template-end.h"

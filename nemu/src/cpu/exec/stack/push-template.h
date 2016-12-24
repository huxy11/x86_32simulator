#include "cpu/exec/template-start.h"

#define instr push

static void do_execute() {
	//TODO
	Log("type = %d val = %d\n", op_src->type, op_src->imm);
}

make_inster_helper(r)

#include "cpu/exec/template-end.h"

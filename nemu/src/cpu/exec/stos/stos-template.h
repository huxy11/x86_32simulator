#include "cpu/exec/template-start.h"

#define instr stos

static void do_execute() {
	panic("to be done");
	print_asm("stos");
}

make_helper(concat(stos_, SUFFIX)) {
	do_execute();
	return 1;
}

#include "cpu/exec/template-end.h"

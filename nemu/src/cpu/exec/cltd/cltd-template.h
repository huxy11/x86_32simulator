#include "cpu/exec/template-start.h"

#define instr cltd

#if DATA_BYTE == 2
#define S_DATA_TYPE int16_t
#elif DATA_BYTE == 4
#define S_DATA_TYPE int32_t
#endif

static void do_execute() {
	S_DATA_TYPE re;
	if ((S_DATA_TYPE)REG(R_EAX) < 0)
		re = -1;
	else 
		re = 0;
	REG(R_EDX) = re;
	print_asm("cltd");
}

make_helper(concat(cltd_,SUFFIX)) {
	do_execute();
	return 1;
}

#undef S_DATA_TYPE
#include "cpu/exec/template-end.h"

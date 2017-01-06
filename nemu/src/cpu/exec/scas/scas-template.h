#include "cpu/exec/template-start.h"

#define instr scas

static void do_execute() {
	DATA_TYPE src = swaddr_read(REG(R_EDI), DATA_BYTE);
	DATA_TYPE dest = REG(R_EAX);
	uint32_t re = dest - src;
	check_pf(re);
	check_sf(re);
	check_zf(re);
	check_sub_cf(src, dest);
	check_sub_of(src, dest, re);
	check_sub_af(src, dest);
	REG(R_EDI) += DATA_BYTE;
	Log("src = %x\t dest = %x\t re = %x\n", src, dest, re);
}

make_helper(concat(scas_, SUFFIX)) {
	do_execute();
	return 1;
}

#include "cpu/exec/template-end.h"

#include "cpu/exec/helper.h"

make_helper(call_rel32) {
	uint32_t re = swaddr_read(eip + 1, 4);
	Log("re = %x\n", re);
	return 5;
}

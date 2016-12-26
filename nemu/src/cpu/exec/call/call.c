#include "cpu/exec/helper.h"

make_helper(call_rel32) {
	int re = swaddr_read(eip + 1, 4);
	Log("re = %d\n", re);
	return 2;
}

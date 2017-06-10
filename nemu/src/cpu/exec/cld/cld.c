#include "cld.h"

make_helper(cld) {
	cpu._df = 0;
	return 1;
	print_asm("cld");
}

#include "std.h"

make_helper(std) {
	cpu._df = 1;
	return 1;
	print_asm("std");
}

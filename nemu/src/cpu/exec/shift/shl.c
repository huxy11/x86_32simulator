#include "cpu/exec/helper.h"

#define DATA_BYTE 2
#include "shl-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "shl-template.h"
#undef DATA_BYTE

make_helper_v(shl_rm_imm)

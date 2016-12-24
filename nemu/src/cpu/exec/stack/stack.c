#include "cpu/exec/helper.h"


#define DATA_BYTE 4
int push_decode(swaddr_t eip) {
	int tmp;
	tmp = instr_fetch(eip + 1, 4);
	
	printf("decode imm = 0x%x\n", tmp);
	return 5;
}
void push_do(void) {
	printf("do!\n");
}

make_helper(push) {
	printf("push\n");
	return idex(eip, push_decode, push_do);
}

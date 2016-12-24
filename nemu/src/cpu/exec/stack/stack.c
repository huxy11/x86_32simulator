#include "cpu/exec/helper.h"


#define DATA_BYTE 4
int push_decode(swaddr_t eip) {
	printf("decode\n");
	return 2;
}
void push_do(void) {
	printf("do!\n");
}

make_helper(push) {
	printf("push\n");
	return idex(eip, push_decode, push_do);
}

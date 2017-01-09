#include "trap.h"
#include "FLOAT.h"

int A[10];
int b;

int main() {
	A[0] = 0;
	A[1] = 1;
	A[2] = 2;
	A[3] = 3;
	A[4] = 4;

	b = A[3];
	A[5] = b;

	FLOAT f = f2F(-1.2);
	nemu_assert(f == 0xfffecccd);

	f = int2F(1);
	nemu_assert(f == 0x10000);

	int i = F2int(0x50000);
	nemu_assert(i == 5);

	nemu_assert(A[0] == 0);
	nemu_assert(A[1] == 1);
	nemu_assert(A[2] == 2);
	nemu_assert(A[3] == 3);
	nemu_assert(A[4] == 4);
	nemu_assert(b == 3);
	nemu_assert(A[5] == 3);
	return 0;
}

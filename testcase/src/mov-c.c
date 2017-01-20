#include "trap.h"
#include "FLOAT.h"

int A[10];
int b;

int test_a(int a,int b, int c,int d, int e, int f, int g)
{
	return a+b+c+d+e+f+g;
}

int main() {
	A[0] = 0;
	A[1] = 1;
	A[2] = 2;
	A[3] = 3;
	A[4] = 4;

	b = A[3];
	A[5] = b;


//	test_a(1,2,3,4,5,6,7);



	nemu_assert(A[0] == 0);
	nemu_assert(A[1] == 1);
	nemu_assert(A[2] == 2);
	nemu_assert(A[3] == 3);
	nemu_assert(A[4] == 4);
	nemu_assert(b == 3);
	nemu_assert(A[5] == 3);
	return 0;
	FLOAT f = f2F(-1.2);
	nemu_assert(f == 0xfffecccd);
	
	f = f2F(0.214285714);
	nemu_assert(f == 0x36db);

	f = int2F(1);
	nemu_assert(f == 0x10000);

	f = Fabs(f2F(-1.2));
	nemu_assert(f == 0x13333);

	int i = F2int(0x50000);
	nemu_assert(i == 5);

	f = F_mul_int(0xfffecccd, 3);
	nemu_assert(f == 0xfffc6667);

	f = F_mul_F(0x13333, 0x59999);
	nemu_assert(f == 0x6b850);

	f = F_mul_F(0xfffecccd, 0x59999);
	nemu_assert(f == 0xfff947af);

	f = F_div_F(0x13333, 0x59999);
	nemu_assert(f == 0x36db);
	return 0;
}

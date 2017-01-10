#include "FLOAT.h"

FLOAT F_mul_F(FLOAT a, FLOAT b) {
	long long rel = (long long)a * (long long)b;
	int *re1 = (int*) &rel;
	int *re2 = re1 + 1;
	int res = ((*re1 & 0xffff0000) >> 16) | ((*re2 & 0xffff) << 16);
	return res;
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
	long long rel = 0;
	int a1 = (a & 0xffff) << 16;
	int a2 = (a & 0xffff0000) >> 16;
	int *re1 = (int*)&rel;
	int *re2 = re1 + 1;

	asm volatile ("idiv %2" : "=a"(*re1), "=d"(*re2): "r"(b), "a"(a1), "d"(a2));
	
	return *re1;
}

FLOAT f2F(float a) {
	int* f = (int*) &a;
	int sign = *f & 0x80000000;
	int ex = *f & 0x7f800000;
	int re = *f & 0x7fffff | 0x800000;

	//float:23.24----0
	//FLOAT:30----16.15----0
	//134 = 127 + 7
	ex >>= 23;
	ex -= 134;
	nemu_assert(ex > -24 && ex < 8);
	if (ex < 0)
		re >>= -ex;
	else
		re <<= ex;

	if (sign)
		re = ~re + 1;
	return re;
}

FLOAT Fabs(FLOAT a) {

	return 0;
}

/* Functions below are already implemented */

FLOAT sqrt(FLOAT x) {
	FLOAT dt, t = int2F(2);

	do {
		dt = F_div_int((F_div_F(x, t) - t), 2);
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

FLOAT pow(FLOAT x, FLOAT y) {
	/* we only compute x^0.333 */
	FLOAT t2, dt, t = int2F(2);

	do {
		t2 = F_mul_F(t, t);
		dt = (F_div_F(x, t2) - t) / 3;
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}


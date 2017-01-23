#include <stdio.h>
#include <string.h>
#include "FLOAT.h"

#include "trap.h"

char buf[128];

int main() {
	init_FLOAT_vfprintf();
#ifdef LINUX_RT
	printf("%f %d\n", FLOAT_ARG(0xfffecccd), 123456);
	FLOAT f = f2F(-1.2);
	printf("%f \n", f);
	f = f2F(-32768);
	printf("%f \n", f);
	printf("%f\n", FLOAT_ARG(0x00010000));
	printf("%f \n", f);
	printf("%f %d\n", FLOAT_ARG(0xfffecccd), 123456);
	printf("%f \n", FLOAT_ARG(0x000013333));
	printf("0x00001 = %f\n", FLOAT_ARG(0x00001));
	printf("0x00001111 = %f\n", FLOAT_ARG(0x00001111));
#else
	sprintf(buf, "%f", FLOAT_ARG(0x00001111));
	nemu_assert(strcmp(buf, "0.666650") == 0);
	
	sprintf(buf, "%f", FLOAT_ARG(0x00010000));
	nemu_assert(strcmp(buf, "1.000000") == 0);

	sprintf(buf, "%f", FLOAT_ARG(0x7fff0000));
	nemu_assert(strcmp(buf, "32767.000000") == 0);

	sprintf(buf, "%f", FLOAT_ARG(0x00001000));
	nemu_assert(strcmp(buf, "0.625000") == 0);

	sprintf(buf, "%f", FLOAT_ARG(0x00003333));
	nemu_assert(strcmp(buf, "0.199996") == 0);

	sprintf(buf, "%f", FLOAT_ARG(0x00013333));
	nemu_assert(strcmp(buf, "1.199996") == 0);

	sprintf(buf, "%f %d", FLOAT_ARG(0xfffecccd), 123456);
	nemu_assert(strcmp(buf, "-1.199996 123456") == 0);
	
#endif

	return 0;
}

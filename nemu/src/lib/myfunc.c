#include <stdlib.h>
#include "nemu.h"

const int BIT[9] = {1, 2, 4, 8, 16, 32, 64, 128, 256};

void my_test(void)
{
	printf("my_test!\n");
}

int tolower_f(int c)
{
	c |= 32; 
	return c;
}	

int htoi(char *h)
{
	int n = 0;
	int i;

	if (h[0] == '0' && h[1] == 'x') 
		i = 2;
	else
	   	i = 0;
	for (;(h[i] >= '0' && h[i] <= '9') || (h[i] >= 'a' && h[i] <= 'z') || (h[i] >= 'A' && h[i] <= 'Z');i++) {
		if (tolower_f(h[i]) > '9')
			n = 16 * n + (10 + tolower_f(h[i]) - 'a');
		else 
			n = 16 * n + (tolower_f(h[i]) - '0');
	}
	return n;
}
int regtoi(char *reg)
{
	reg++;
	int i = 0;
	for (;i < 8; i++) {
		if (!strcmp(reg, regsl[i]))
			return reg_l(i);
		if (!strcmp(reg, regsw[i]))
			return reg_w(i);
		if (!strcmp(reg, regsb[i]))
			return reg_b(i);
		Log("%s\t%s\t%s\t%s\n", reg, regsl[i], regsw[i], regsb[i]);
	}
	panic("Wrong register name!");
	return 0;
}
/*
int str2x(char *s)
{
	char *str;
	strcpy(str, s);
	int i = 0;
	for (;;i++) {
		str[i] &= BIT[6];
		
	}
}
*/

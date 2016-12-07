#include <stdlib.h>
#include <stdio.h>

int tolower(int c)
{
	return c |= 32; 
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
		if (tolower(h[i]) > '9')
			n = 16 * n + (10 + tolower(h[i]) - 'a');
		else 
			n = 16 * n + (tolower(h[i]) - '0');
	}
	return n;
}

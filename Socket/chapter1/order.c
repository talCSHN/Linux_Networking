#include <stdio.h>

void main()
{
	int n = 0x1234567;
	char* pn = &n;
	printf("1st: %p, %#x\n", &(*pn), *pn);
}

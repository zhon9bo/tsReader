#include <stdio.h>
#include "opration.h"

int main()
{
	unsigned char psi[1024];
	FILE * ts;
	int size;

	ts = fopen("100.ts","rb");
	printf("PAT:\n");
	size = getpsi(0, 1, ts, psi);
	if(size != -1)
		printbufhex(psi, size, 16);
}

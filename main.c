#include <stdio.h>
#include "opration.h"
#include "table/pat.h"

int main()
{
	unsigned char psi[1024];
	FILE * ts;
	int size;
	PSI_PAT pat;

	ts = fopen("100.ts","rb");
	printf("PAT:\n");
	size = getpsi(0, 1, ts, psi);
	if(size != -1)
		printbufhex(psi, size, 16);
	parse_pat(psi, &pat);
	show_pat(&pat);
}

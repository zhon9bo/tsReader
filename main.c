#include <stdio.h>
#include <string.h>
#include "opration.h"
#include "table/pat.h"
#include "table/pmt.h"

int main()
{
	unsigned char psi[1024];
	FILE * ts;
	int size;
	unsigned int i;
	PSI_PAT pat;
	PSI_PMT pmt;

	ts = fopen("100.ts","rb");
	printf("PAT:\n");
	memset(psi,0,1024);
	size = getpsi(0, 1, ts, psi);
	if(size != -1)
		printbufhex(psi, size, 16);
	parse_pat(psi, &pat);
	show_pat(&pat);

	for(i = 0; i < pat.prg_count; i++)
	{	
		printf("\nprogram %d/%d\n",i+1,pat.prg_count);
		memset(psi,0,1024);
		size = getpsi(pat.prorams[i].map_pid, 1, ts, psi);
		if(size == -1)
			return -1;
		printbufhex(psi, size, 16);
		parse_pmt(psi, &pmt);	
		show_pmt(&pmt);
	}

	size = getpsi(0x11, 1, ts, psi);
	if(size == -1)
		return -1;
	printbufhex(psi, size, 16);
	return 0;
}

#include <stdio.h>

int main()
{
	FILE *f;
	unsigned char buf;
	f = fopen("adaptition.ts","rb");
	
	fread(&buf,1,1,f);
	printf("%#x\n",buf);
	
	fseek(f, 188, SEEK_CUR)	;
	
	fread(&buf,1,1,f);
	printf("%#x\n",buf);
	
	fread(&buf,1,1,f);
	printf("%#x\n",buf);
	
	fseek(f, 1, SEEK_CUR)	;
	
	fread(&buf,1,1,f);
	printf("%#x\n",buf);	
	
	fseek(f, -1, SEEK_CUR);
	
	fread(&buf,1,1,f);
	printf("%#x\n",buf);	
}

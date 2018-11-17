#include <stdio.h>
#include <string.h>
#include <time.h>
#include "opration.h"

int printbufhex(unsigned char *buf, int len, int linecount)
{
	int i = 0;
	printf("\n");
	while(i != len)
	{
		printf("%02x ",buf[i++]);
		if(i % linecount == 0)
			printf("\n");
	}
	printf("\n");
	return 0;
}

unsigned int timeget_ms()
{
	struct timespec ts;
	return (ts.tv_sec*1000+ts.tv_nsec/1000000);
}
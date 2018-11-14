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

int get_tsPkt_byNum(FILE *ts_stream, unsigned char *pkt, int num)
{
    unsigned int index = 0;
    unsigned char sync_head;
    unsigned int end  = num+1;
    while(index == end)
    {
        while(sync_head != 0x47)
        {	
            fread(&sync_head, 1, 1, ts_stream);
            index++;
        }
        if(index != end)
        {
           fseek(ts_stream, 188, SEEK_CUR); 
        }
    }
    fread(pkt, 188, 1, ts_stream);
    //printbufhex(pkt,188,16);
    return 1;
}

int get_tsPkt_byPID(FILE *ts_stream, unsigned char *pkt, int num, int pid)
{
    int i = num;
    int pid_get;

    for(i = num; i<200000; i++)
    {
        get_tsPkt_byNum(ts_stream, pkt, i);
        pid_get = (((unsigned int )pkt[1]&0x1f)<<8)|pkt[2];
        if(pid_get == pid)
            return i;
    }

}


int getpsi(unsigned int PID, unsigned int time_out, FILE *ts_file, unsigned char *psi)
{
	int i = 0 ;
	unsigned char pkt[188];
	int data_pos;
	int pmt_section_len;
	int pos;
	int remain;
	unsigned int last_time;
	unsigned int current_time;

	last_time = timeget_ms();
	current_time = timeget_ms();
	while((current_time - last_time) < time_out)
	{
		current_time = timeget_ms();
		i = get_tsPkt_byPID(ts_file, pkt,i,PID);

		if(adaptation_field_control(pkt) == ADAPTITION_EXIST)
			data_pos = adpatition_size(pkt)+5;
		else if(adaptation_field_control(pkt) == ADAPTITION_NOT)
			data_pos = 4;

		if(payload_unit_start_indicator(pkt) == 1)//first pkt
		{
			data_pos += 1;
			//printbufhex((&pkt[data_pos]),(188 - data_pos),16);
			pmt_section_len = section_length((&pkt[data_pos]));
			remain = pmt_section_len - (188 - data_pos -3);
			if(remain <= 0)
			{	
				memcpy(psi, &pkt[data_pos], pmt_section_len+3);
				pos = pmt_section_len+3;
				goto ok;
			}
			else
			{
				memcpy(psi, &pkt[data_pos], (188 - data_pos));
				pos = 188 - data_pos;
			}
		}
		else
		{
			remain = remain - (188 - data_pos);
			if(remain <= 0)
			{
				memcpy(psi+pos, &pkt[data_pos], remain + (188 - data_pos));
				pos += remain + (188 - data_pos);
				goto ok;
			}
			else
			{
				memcpy(psi+pos, &pkt[data_pos], (188 - data_pos));
				pos += 188 - data_pos;
			}
			
		}
	}
	printf("time out\n");
	return -1;	
ok:
	//printbufhex(psi,pos,16);	
	return pos;
}
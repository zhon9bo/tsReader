#include <stdio.h>
#include "pmt.h"
int parse_pmt(unsigned char *pmt, PSI_PMT *psi_pmt)
{
    int section_length;
    int es_length;
    int es_count = 0;
    unsigned char *cur_es;
    int remain_len;

    psi_pmt->prg_info_len   = program_info_length(pmt);
    psi_pmt->pcr_pid        = PCR_PID(pmt);
    psi_pmt->program_number = pmt_program_number(pmt);

    section_length = pmt_section_length(pmt);
    es_length = section_length - 9 - program_info_length(pmt) - 4;
    remain_len = es_length;
    cur_es = FIRST_ES_HEAD(pmt);
    while(remain_len > 0)
    {
        es_count++;
        psi_pmt->es[es_count-1].stream_type = stream_type(cur_es);
        psi_pmt->es[es_count-1].pid = elementary_PID(cur_es);
        psi_pmt->es[es_count-1].es_info_len = ES_info_length(cur_es);
        remain_len -= ES_info_length(cur_es) + 5;
        cur_es = NEXT_ES_HEAD(cur_es);
    }
    psi_pmt->es_count = es_count;

    return 0;
}

int show_pmt(PSI_PMT *psi_pmt)
{
    int es_count = psi_pmt->es_count;
    int i = 0;
    printf("program number:%#x(%d)\n",psi_pmt->program_number,psi_pmt->program_number);
    printf("pcr pid:%#x(%d)\n",psi_pmt->pcr_pid,psi_pmt->pcr_pid);
    printf("program_info_length:%#x(%d)\n",psi_pmt->prg_info_len,psi_pmt->prg_info_len);

    while(es_count--)
    {
        printf("\n\tstream type:%#x(%d)\n",psi_pmt->es[i].stream_type,psi_pmt->es[i].stream_type);
        printf("\tpid:%#x(%d)\n",psi_pmt->es[i].pid,psi_pmt->es[i].pid);
        printf("\tes_info_len:%#x(%d)\n",psi_pmt->es[i].es_info_len,psi_pmt->es[i].es_info_len);
        i++;
    }

    return 0;
}
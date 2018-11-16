#include <stdio.h>
#include "pat.h"
 
int parse_pat(unsigned char *pat , PSI_PAT *psi_pat)
{
    unsigned int N;
    unsigned int i;
    if(pat == NULL)
        return -1;

    if(table_id(pat) != 0)
        return -1;

    if((pat_section_length(pat) - 9)&0x3)
    {
        printf("pat_section_length Error!\n");
        return -1;
    }

    N = (pat_section_length(pat) - 9) >> 2;
    psi_pat->prg_count = N;
    for(i = 0; i < N; i++)
    {
       psi_pat->prorams[i].program_number = program_number(pat,i);
       psi_pat->prorams[i].map_pid = map_pid(pat,i);
    }

    return 0;
}

int show_pat(PSI_PAT *psi_pat)
{
    int i;
    for(i = 0; i < psi_pat->prg_count; i++)
    {
        printf("program_number:%#x\n",psi_pat->prorams[i].program_number);
        printf("map_pid       :%#x\n",psi_pat->prorams[i].map_pid);
    }
    return 0;
}
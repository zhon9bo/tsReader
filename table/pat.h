#ifndef _PAT_H_
#define _PAT_H_

#define PAT_PRG_MAX 252

//PAT Head 8 byte
#define table_id(pat)                    (pat[0])
#define section_syntax_indicator(pat)    ((pat[1]>>7)&0x1)
#define pat_section_length(pat)              (((pat[1]&0xf)<<8)|pat[2])
#define transport_stream_id(pat)         ((pat[3]<<8) | pat[4])
#define version_number(pat)              ((pat[5]>>1)&0x1f)  
#define current_next_indicator(pat)      (pat[5]&0x1)
#define section_number(pat)              (pat[6])
#define last_section_number(pat)         (pat[7])

#define program_number(pat,N)        ((pat[8+4*N]<<8)|pat[9+4*N])
#define map_pid(pat,N)               (((pat[10+4*N]&0x1f)<<8)|pat[11+4*N])

typedef struct{
    unsigned short program_number;
    unsigned int map_pid;
    void * pmt;
}PAT_PRG;

typedef struct{
    unsigned int prg_count;
    PAT_PRG prorams[PAT_PRG_MAX];
}PSI_PAT;

extern int parse_pat(unsigned char *pat , PSI_PAT *psi_pat);
extern int show_pat(PSI_PAT *psi_pat);
#endif
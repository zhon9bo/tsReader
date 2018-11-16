#ifndef _PMT_H_
#define _PMT_H_

#define pmt_section_length(pmt)              (((pmt[1]&0xf)<<8)|pmt[2])
#define pmt_program_number(pmt)              ((pmt[3]<<8) | pmt[4])
#define PCR_PID(pmt)                     (((pmt[8]&0x1f)<<8)|pmt[9])
#define program_info_length(pmt)         (((pmt[10]&0xf)<<8)|pmt[11])
#define FIRST_ES_HEAD(pmt)               (&pmt[12+program_info_length(pmt)])

//ES INFO
#define stream_type(cur_es)             (cur_es[0])
#define elementary_PID(cur_es)          (((cur_es[1]&0x1f)<<8)|cur_es[2])
#define ES_info_length(cur_es)          (((cur_es[3]&0xf)<<8)|cur_es[4])
#define NEXT_ES_HEAD(cur_es)            (&cur_es[5 + ES_info_length(cur_es)])

typedef struct{
    unsigned char stream_type;
    unsigned int pid;
    unsigned int es_info_len;
    unsigned char *descriptor;
}ES_INFO;

typedef struct{
    unsigned int program_number;
    unsigned int pcr_pid;
    unsigned int es_count;
    unsigned int prg_info_len;
    unsigned char *descriptor;
    ES_INFO   es[200];
}PSI_PMT;

extern int parse_pmt(unsigned char *pmt, PSI_PMT *psi_pmt);
extern int show_pmt(PSI_PMT *psi_pmt);
#endif
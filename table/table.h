#ifndef _TABLE_H_
#define _TABLE_H_

#include "../list/list.h"

#define sync_byte(pkt)                      (pkt[0])
#define transport_error_indicator(pkt)      ((pkt[1]>>7)&0x1)
#define payload_unit_start_indicator(pkt)   ((pkt[1]>>6)&0x1)
#define transport_priority(pkt)             ((pkt[1]>>5)&0x1)
#define PID(pkt)                            ((pkt[1]<<8|pkt[2])&0x1fff)
#define transport_scrambling_control(pkt)   ((pkt[3]>>6)&0x3)
#define adaptation_field_control(pkt)       ((pkt[3]>>4)&0x3)
#define continuity_counter(pkt)             (pkt[3]&0xf)

#define GET_CRC(buf,buf_len)                    (buf[buf_len -4]<<24|buf[buf_len -3]<<16|buf[buf_len -2]<<8|buf[buf_len -1])

#define ADAPTITION_RESEVE   0
#define ADAPTITION_NOT      1
#define ADAPTITION_ALL      2
#define ADAPTITION_EXIST    3

#define POINTTER_FIELD_EXIST 1
#define POINTTER_FIELD_NOT   0

#define PKT_HEAD_LEN        4

typedef struct{
    unsigned int section_number;
    unsigned int section_length;
    unsigned char *section;
    struct list_head list;
}SECTION;


extern int del_sections(struct list_head *list_head);
extern int TABLE_Packets2Section(char *input_name, unsigned int timeout_ms, 
    unsigned int pid, unsigned int table_id,struct list_head *sections_head);
#endif
#ifndef _OPRATION_H_
#define _OPRATION_H_

//ts head
#define sync_byte(pkt)                      (pkt[0])
#define transport_error_indicator(pkt)      ((pkt[1]>>7)&0x1)
#define payload_unit_start_indicator(pkt)   ((pkt[1]>>6)&0x1)
#define transport_priority(pkt)             ((pkt[1]>>5)&0x1)
#define PID(pkt)                            (((pkt[1]&0xf)<<8)|pkt[2])
#define transport_scrambling_control(pkt)   ((pkt[3]>>6)&0x3)
#define adaptation_field_control(pkt)       ((pkt[3]>>4)&0x3)
#define continuity_counter(pkt)             (pkt[3]&0xf)


#define adpatition_size(pkt)                (pkt[4])

#define ADAPTITION_RESEVE   0
#define ADAPTITION_NOT      1
#define ADAPTITION_ALL      2
#define ADAPTITION_EXIST    3

//pmt
#define section_length(pmt)                 (((pmt[1]&0xf)<<8)|pmt[2])

int printbufhex(unsigned char *buf, int len, int linecount);
int getpsi(unsigned int PID, unsigned int time_out, FILE *ts_file, unsigned char *psi);
#endif
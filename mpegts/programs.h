#ifndef _PROGRAM_H_
#define _PROGRAM_H_

typedef struct CHANNEL{
    char * channle_name;
    unsigned int network_id;
    unsigned int original_network_id;
    struct PROGRAM *programs;
    struct CHANNEL *next;
}CHANNEL;

typedef struct PROGRAM{
    unsigned int program_number;
    unsigned int pmt_pid;
    unsigned int pcr_pid;
    unsigned int program_info_length;
    struct DESC *program_desc;//pmt 中的节目描述
    struct ELEM_STREM *elem_streams;
    unsigned char EIT_schedule_flag;
    unsigned char EIT_present_following_flag;
    unsigned char runing_status;
    unsigned char free_CA_mode;
    unsigned int  service_descriptor_length;
    struct DESC *service_descriptor;//sdt中
    struct PROGRAM *next;
}PROGRAM;

typedef struct DESC{
    unsigned char descriptor_tag;
    unsigned int  descriptor_length;
    unsigned char *descriptor_data;
    struct DESC *next;
}DESC;

typedef struct ES{
    unsigned char stream_type;
    unsigned int  elem_pid;
    unsigned int  es_info_length;
    struct DESC *es_desc;
    struct ELEM_STREM *next;
}ELEM_STREM;

extern int DESC_Del_All(DESC *desc);
extern DESC *DESC_New(unsigned int descriptor_tag, unsigned int descriptor_length);
extern int DESC_Add(DESC **descs, DESC *desc_new);
extern int DESC_Show(DESC *desc);

extern int ELEM_STREM_Del_All(ELEM_STREM *elem_stream);
extern ELEM_STREM *ELEM_STREM_New(unsigned int stream_type, unsigned int elem_pid, unsigned int es_info_length);
extern ELEM_STREM *ELEM_STREM_FindBy_PID(ELEM_STREM *elem_stream, unsigned int elem_pid);
extern int ELEM_STREM_Add(ELEM_STREM **elem_streams, ELEM_STREM *elem_stream_new);
extern int ELEM_STREM_Show(ELEM_STREM *elem_stream);

extern int PROGRAM_Del_All(PROGRAM *program);
extern PROGRAM *PROGRAM_New(unsigned int program_number, unsigned int map_pid);
extern PROGRAM *PROGRAM_FindBy_PrgNumber(PROGRAM *program, unsigned int program_number);
extern int PROGRAM_Add(PROGRAM **programs, PROGRAM *program_new);
extern int PROGRAM_Show(PROGRAM *program);
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "pmt.h"
#include "table.h"
#include "../mpegts/programs.h"
#include "descriptors.h"

int parse_pmt(struct list_head *sections_head, PROGRAM *programs)
{
    ELEM_STREM *elem_stream;
    ELEM_STREM *elem_stream_new;
    SECTION *section;
    unsigned char *section_buf;
    unsigned int pos;
    unsigned int stream_type;
    unsigned int elem_pid;
    unsigned int pcr_pid;
    unsigned int program_info_length;
    unsigned int es_info_length;

    if(sections_head == NULL || programs == NULL)
        return -1;

    if(list_empty(sections_head))
        return -1;

    elem_stream = programs->elem_streams;
       
    if(elem_stream != NULL)
    {
        printf("删除所有elem stream\n");
        ELEM_STREM_Del_All(elem_stream);
    }

    list_for_each_entry(section, sections_head, list)
    {
        section_buf = section->section;
        pos = 8;
        
        pcr_pid = (section_buf[pos]<<8|section_buf[pos+1])&0x1fff;
        program_info_length = (section_buf[pos+2]<<8|section_buf[pos+3])&0xfff;

        programs->pcr_pid = pcr_pid;
        programs->program_info_length = program_info_length;

        /*处理描述符*/ 
        parse_descriptor(&(programs->program_desc), &section_buf[pos+4], program_info_length);

        pos += 4 + program_info_length;

        while((pos+9) <= section->section_length)
        {
            stream_type    = section_buf[pos];
            elem_pid       = (section_buf[pos+1]<<8 | section_buf[pos+2])&0x1fff;
            es_info_length = (section_buf[pos+3]<<8 | section_buf[pos+4])&0xfff;

            elem_stream_new = ELEM_STREM_FindBy_PID(elem_stream,elem_pid);
            if(elem_stream_new == NULL)
            {
                elem_stream_new = ELEM_STREM_New(stream_type, elem_pid, es_info_length);
                if(elem_stream_new != NULL)
                {   
                    /*处理描述符*/   
                    parse_descriptor(&(elem_stream_new->es_desc), &section_buf[pos+5], es_info_length);
                    ELEM_STREM_Add(&(programs->elem_streams),elem_stream_new);
                }
            }
            else
            {
                printf("存在相同PID的Elem Stream\n");
            }

            pos += 5 + es_info_length;
        }
    }

    return 0;
}
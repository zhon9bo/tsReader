#include <stdio.h>
//#include "sdt.h"
#include "../mpegts/programs.h"
#include "table.h"
#include "descriptors.h"

int parse_sdt(struct list_head *sections_head, CHANNEL *channel)
{
    PROGRAM *program;
    SECTION *section;
    unsigned char *section_buf;
    unsigned int pos;
    unsigned int original_network_id;
    unsigned int service_id;
    unsigned char EIT_schedule_flag;
    unsigned char EIT_present_following_flag;
    unsigned char runing_status;
    unsigned char free_CA_mode;
    unsigned int descriptors_loop_length;

    list_for_each_entry(section, sections_head, list)
    {
        section_buf = section->section;
        pos = 8;

        original_network_id = (section_buf[pos]<<8|section_buf[pos+1])&0xffff;
        channel->original_network_id = original_network_id;
        pos += 3; 
        while((pos + 4) < section->section_length)//有点问题
        {
            service_id                 = (section_buf[pos]<<8|section_buf[pos+1])&0xffff;//program_number
            EIT_schedule_flag          = (section_buf[pos+2]>>1)&0x1;
            EIT_present_following_flag =  section_buf[pos+2]&0x1;
            runing_status              = (section_buf[pos+3]>>5)&0x3;
            free_CA_mode               = (section_buf[pos+3]>>4)&0x1;
            descriptors_loop_length    = (section_buf[pos+3]<<8|section_buf[pos+4])&0xfff;

            program = PROGRAM_FindBy_PrgNumber(channel->programs,service_id);
            if(program != NULL)
            {
                program->EIT_schedule_flag          = EIT_schedule_flag;
                program->EIT_present_following_flag = EIT_present_following_flag;
                program->runing_status              = runing_status;
                program->free_CA_mode               = free_CA_mode;
                program->service_descriptor_length  = descriptors_loop_length;

                parse_descriptor(&(program->service_descriptor), &section_buf[pos+5], descriptors_loop_length);
            }
            else
            {
                printf("不存在service id 为%#X(%d)的节目\n",service_id,service_id);
            }
            pos += 5 + descriptors_loop_length;
        }
    }
}
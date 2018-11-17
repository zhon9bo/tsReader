#include <stdio.h>
#include <stdlib.h>
#include "pat.h"
#include "../mpegts/programs.h"
#include "table.h"

int parse_pat(struct list_head *sections_head, CHANNEL *channel)
{
    PROGRAM *programs;
    PROGRAM *program_new;
    SECTION *section;
    unsigned char *section_buf;
    unsigned int program_number;
    unsigned int map_pid;
    unsigned int pos;
    unsigned int network_id = 0;

    if(sections_head == NULL || channel == NULL)
        return -1;

    if(list_empty(sections_head))
        return -1;

    programs = channel->programs;
       
    if(!mylist_empty(programs))
    {
        printf("删除所有节目\n");
        PROGRAM_Del_All(programs);
    }

    list_for_each_entry(section, sections_head, list)
    {
        section_buf = section->section;
        pos = 8;
        while((pos + 4) < section->section_length)
        {
            program_number = (section_buf[pos]<<8 | section_buf[pos+1])&0xffff;
            map_pid = (section_buf[pos+2]<<8 | section_buf[pos+3])&0x1fff;

            if(program_number == 0)
            {
                network_id = map_pid;
                channel->network_id = network_id;
            }    
            else
            {
                program_new = PROGRAM_FindBy_PrgNumber(programs, program_number);
                if(program_new == NULL)
                {
                    program_new = PROGRAM_New(program_number, map_pid);
                    PROGRAM_Add(&channel->programs, program_new);
                    printf("添加一个节目program number %#X(%d) map_pid %#X(%d)\n",program_number, program_number, map_pid, map_pid);
                }
                else
                {
                    printf("存在与program number %#X(%d) map_pid %#X(%d)相同的节目\n",program_number, program_number, map_pid, map_pid);
                }

            }
            pos += 4;
        }
    }

    return 0;
}
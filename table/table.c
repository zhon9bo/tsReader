#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"
#include "../input/input.h"
#include "../opration.h"
#include "../cytpto/crc32.h"

int section_number_exist(struct list_head *list_head, unsigned int section_number)
{

    SECTION *section;

    if(list_head == NULL)
        return -1;

    list_for_each_entry(section, list_head, list)
    {
        if(section->section_number == section_number)
            return 1;
    }

    return 0;
}


int show_section(struct list_head *list_head)
{
    SECTION *section;

    if(list_head == NULL)
    {
        return -1;
    }

    list_for_each_entry(section, list_head, list)
    {
        printf("section number:%#X(%d)",section->section_number,section->section_number);
        printbufhex(section->section, section->section_length, 16);
    }

    return 0;
}

SECTION *new_section(unsigned int section_length, unsigned int section_number)
{
    SECTION * section;

    if(section_length < 5)
    {
        printf("错误的长度\n");
        return NULL;
    }

    section = malloc(sizeof(SECTION));
    if(section == NULL)
    {
        printf("分配段链表错误\n");
        return NULL;
    }

    section->section = malloc(section_length);
    if(section->section == NULL)
    {
        free(section);
        printf("分配section的内存错误\n");
        return NULL;
    }

    section->section_number = section_number;
    section->section_length = section_length;

    //printf("成功分配setion, 大小: %d\n",section_length);

    return section;
}

int add_section(struct list_head *list_head, SECTION *section)
{  

    if(list_head == NULL || section == NULL)
    {
        return -1;
    }

    if(section->section_length == 0 || section->section ==NULL)
    {
        printf("add a invalid section!\n");
        return -1;
    }
    
    if(section_number_exist(list_head, section->section_number) == 1)
    {
        printf("[Warning] section number exist ! add faild\n");
        return -1;
    }

    list_add_tail(&section->list,list_head);

    return 0;
}

int del_section(SECTION *section)
{
    if(section == NULL)
    {
        printf("需要删除的指针为null\n");
        return -1;
    }

    if(section->section == NULL)
    {
        printf("需要删除的section为null\n");
    }
    else
    {
        free(section->section);
        printf("成功释放section, 大小: %d\n",section->section_length);
    }

    free(section);
    
    return 0;
}

int del_sections(struct list_head *list_head)
{
    SECTION *section;
    SECTION *section_temp;

    if(list_head == NULL)
    {
        return -1;
    }

    list_for_each_entry_safe(section,section_temp,list_head, list)
    {
        if(section->section != NULL)
        {
            free(section->section);
            //printf("del section number:%#X(%d)\n",section->section_number,section->section_number);
        }

        list_del(&section->list);
        free(section);
    }

    return 0;
}
  
int TABLE_Packets2Section(char *input_name, unsigned int timeout_ms, 
    unsigned int pid, unsigned int table_id,struct list_head *sections_head)
{
    unsigned int time_now;
    unsigned int time_old;
    unsigned int pos;
    unsigned int adaptation_field_len;
    unsigned int point_field_len;
    unsigned int section_length = 0;
    unsigned int section_number;
    unsigned int last_section_number = 0;
    unsigned int all_length;
    unsigned int cur_pkt_remain;
    unsigned int copy_pos;
    unsigned int copy_len;
    unsigned int section_count = 0;
    unsigned char pkt[188];
    unsigned int CRC;
    unsigned int CRC_calc;
    SECTION * section = NULL;

    if(sections_head == NULL)
        return -1;

    time_old = timeget_ms();
    time_now = timeget_ms();

    INIT_LIST_HEAD(sections_head);

    while(section_count != last_section_number+1)
    {
        if((time_now - time_old) > timeout_ms)//判断是否超时
        {
            if(section)
                del_section(section);
            
            printf("解析超时\n");
            return -1;
        }
        
        if(INPUT_GetPkt_From(input_name,pkt,188,1) != 0)
            return -1;

        if(PID(pkt) != pid)
            continue;

        /*判断调整字段，计算调整字段后的偏移pos*/
        if(adaptation_field_control(pkt) == ADAPTITION_EXIST)
        {
            adaptation_field_len = pkt[4];
            pos = adaptation_field_len + PKT_HEAD_LEN + 1;
        }
        else if(adaptation_field_control(pkt) == ADAPTITION_NOT)   
            pos = PKT_HEAD_LEN;
        
        /*判断是否存在 pointer_feild */
        if(payload_unit_start_indicator(pkt) == POINTTER_FIELD_EXIST)
        {
            point_field_len = pkt[pos];
            pos += point_field_len + 1;

            /*判断table id 是否相等*/
            if(pkt[pos] != table_id)
            {
                printf("table id 不相等\n");
                continue;
            }
            
            /*如果一个section没有复制完成(没有添加到链表中)就读取到了新的一个section*/
            if(section != NULL)
            {    
                del_section(section);
                section == NULL;
            }

            /*获取表头部信息*/
            section_length      = ((pkt[pos+1]<<8)|pkt[pos+2])&0xfff;
            section_number      = pkt[pos+6];
            last_section_number = pkt[pos+7];

            all_length = section_length + 3;//加上头部的长度

            section = new_section(all_length, section_number);//分配保存section的地址
            if(section == NULL)
                return -1;

            copy_pos = 0;//从开始复制
            copy_len = 0;//已经复制了0字节 
        }

        if(section_length == 0)//必须先解析第一个包
            continue;

        cur_pkt_remain = 188 - pos;//当前包剩余的长度

        if(cur_pkt_remain > (all_length - copy_pos))//最后一个包
            copy_len = all_length - copy_pos;
        else
            copy_len = cur_pkt_remain;

        memcpy(section->section + copy_pos, &pkt[pos], copy_len);
        copy_pos += copy_len;

        if(copy_pos == all_length)//复制完成
        {
            /*计算crc*/
            CRC = GET_CRC(section->section,section->section_length);
            CRC_calc = Calc_CRC32(section->section,section->section_length - 4);

            if(CRC != CRC_calc)
            {
                printf("CRC 错误 %#X %#X\n",CRC, CRC_calc);
                del_section(section);
            }
            else
            {
                if(add_section(sections_head,section) != 0)//把一个完整的段放入链表
                    del_section(section);
                else
                    section_count++;
            }

            section = NULL;
            section_length = 0;
        }   
    }

    //printf("成功获取pid：%d, table id:%d, 的所有Section, 个数：%d\n", pid, table_id,section_count);

    return 0;
}
#include "input.h"
#include "tsfile/tsfile.h"
#include <stdio.h>
#include <string.h>

static struct list_head g_inputlist;

int INPUT_Register(INPUT *input)
{
    int ret;

    list_add_tail(&input->list,&g_inputlist);
    ret = list_is_last(&input->list,&g_inputlist);

    return ret;
}

int INPUT_GetPkt_From(const char *name, unsigned char *pkts, unsigned int size, unsigned int count)
{
    INPUT *input;
    
    if(name == NULL || pkts == NULL || size == 0 || count == 0)
        return -1;

    if(list_empty(&g_inputlist))
    {
        printf("[Error] no input!\n");
        return -1;
    }  

    list_for_each_entry(input, &g_inputlist, list)
    {
        if(strcmp(input->name, name) != 0)
            continue;

        //printf("get pkt from %s\n",name);
        return input->Get_Pkt(pkts,size,count);
    }

    printf("[Warning] no this input, Check name\n");
    return -1;

}

int INPUT_Get_Channel(const char *name, INPUT *input)
{
    INPUT *tmp_input;

    if(input == NULL || name == NULL)
    {
        printf("[Error] invalid input parameters!\n");
        return -1;
    }

    if(list_empty(&g_inputlist))
    {
        printf("[Error] no input!\n");
        return -1;
    }   
    
    list_for_each_entry(tmp_input, &g_inputlist, list)
    {
        if(strcmp(tmp_input->name, name) != 0)
            continue;

        *input = *tmp_input;
        return 0;
    }

    printf("[Warning] no this input, Check name\n");
    return -1;
}

int INPUT_AllChannel_Init()
{
    INPUT *input;
    int ret = 0;

    if(list_empty(&g_inputlist))
    {
        printf("[Error] no input!\n");
        return -1;
    }

    list_for_each_entry(input, &g_inputlist, list)
    {
        if(input->Init() == 0)
            continue;
        printf("[Error] %s init failed\n",input->name);
        ret++;
    }

    return ret;
}


int INPUT_Init()
{
    int ret;

    /*初始化输入链表*/
    INIT_LIST_HEAD(&g_inputlist);

    /*输入方式初始化*/
    ret = TSFILE_Init();

    //ret |= UDPInput_Init();

    return ret;
}
#include <stdio.h>
#include <string.h>
#include "../input.h"

#define TSFILE_NAME "input/tsfile/adaptition.ts"

static FILE *g_ts;

static int IntputFile_Init()
{
    g_ts = fopen(TSFILE_NAME,"rb");

    fseek(g_ts,0,SEEK_SET);
    if(g_ts == NULL)
    {
        printf("[Error] open %s failed!\n",TSFILE_NAME);
        ferror(g_ts);
        return -1;
    }

    return 0;
}
#if 0
static int check_pakt(unsigned char *pkt, unsigned int size)
{
    if(pkt[0] != 0x47)
        return -1;
    
    
    /*计算CRC等*/

}
#endif
/*获取包*/
static int IntputFile_Get_Pkt(unsigned char *pkts, unsigned int pkt_size, unsigned int pkt_count)
{
    unsigned char head = 0;
    unsigned int  count = pkt_count;
    unsigned int  success_times;
    unsigned int  pos = 0;

    if(pkts == NULL || count == 0 || pkt_size == 0 )
        return -1;
    
    memset(pkts, 0, pkt_size*pkt_count);

    while(pos != (count * pkt_size))
    {   
        head = 0;
        while(head != 0x47)
        {
            if(fread(&head,1,1, g_ts) != 1)
            {
                printf("[Error] Read Error!");
                return -1;
            }
        }
        fseek(g_ts,-1,SEEK_CUR);//返回头部
        success_times = 5;

        while(success_times)//检查是否连续5个pkt的头部都为0x47
        {
            success_times--;
            fseek(g_ts,pkt_size,SEEK_CUR);//移动到下一个包
            head = 0;
            if(fread(&head,1,1, g_ts) != 1)
                return -1;
            fseek(g_ts,-1,SEEK_CUR);//返回包的开始
            if(head != 0x47)
            {
                printf("下一个188的头部为 %#x\n",head);
                count++;
                break;
            }
            //printf("成功%d个包\n",5-success_times);
        }

        fseek(g_ts,(int)((success_times-5)*pkt_size),SEEK_CUR);//返回第一个包的头部

        if(success_times == 0)//连续5个pkt的头部都为0x47
        {   
            fread(pkts+pos, 1, pkt_size, g_ts);
            pos += pkt_size;
        }
        else
        {
            printf("不是一个正确的包 %d\n",success_times);
        }
    }

    return 0;
}

/*通过传入的json数据设置当前输入的参数*/
static int IntputFile_Set_Parameters()
{

    return 0;
}

static INPUT g_InputFile = {
    .name           = "tsfile",
    .Init           = IntputFile_Init,
    .Get_Pkt        = IntputFile_Get_Pkt,
    .Set_Parameters = IntputFile_Set_Parameters,
};

int TSFILE_Init()
{
    return INPUT_Register(&g_InputFile);
}
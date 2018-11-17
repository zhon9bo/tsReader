#ifndef _INPUT_H_
#define _INPUT_H_
#include "../list/list.h"

typedef struct INPUT_ST{
    char *name;
    struct list_head list;
    int (*Init)(void);
    int (*Get_Pkt)(unsigned char *pkts, unsigned int size, unsigned int count);
    int (*Get_Parameters)();
    int (*Set_Parameters)();
}INPUT;

extern int INPUT_Init();
extern int INPUT_AllChannel_Init();
extern int INPUT_Register(INPUT *input);
extern int INPUT_Get_Channel(const char *name, INPUT *input);
extern int INPUT_GetPkt_From(const char *name, unsigned char *pkts, unsigned int size, unsigned int count);

#endif
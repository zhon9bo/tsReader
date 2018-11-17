#ifndef _BITHANDLE_H_
#define _BITHANDLE_H_

#define BITHANDLE_ERROR   printf
#define BITHANDLE_WARNING printf
#define BITHANDLE_MESSAGE printf

/************************************************
 *mask:获取1bit的掩码
 *pos：第几个字节
 *bits：使用了多少个字节
 ************************************************/
typedef struct{
    unsigned char *buf;
    unsigned char mask;
    unsigned char bits;
    unsigned int  bufsize;
    unsigned int  pos;
}BIT_BUF;

extern int BitHandle_Init(BIT_BUF *bit_buf, void *buf, unsigned int buf_size);
extern int BitHandle_Set(BIT_BUF *bit_buf, unsigned int value, unsigned char bits);
extern int BitHandle_Get(BIT_BUF *bit_buf, unsigned int *value, unsigned char bits);
#endif
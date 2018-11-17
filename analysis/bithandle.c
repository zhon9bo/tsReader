/************************************************
 *初始化BIT_BUF 将要处理的buf指针和大小初始化
 *mask移动到头部0x80(1000 0000)
 ************************************************/
int BitHandle_Init(BIT_BUF *bit_buf, void *buf, unsigned int bufsize)
{
    if(bit_buf == NULL || buf == NULL || bufsize == 0)
    {
        BITHANDLE_ERROR("[Error] BitHandle init error,Invalid input parameters!\n");
        return -1;
    }

    bit_buf->pos  = 0;
    bit_buf->bits = 0;
    bit_buf->mask = 0x80;
    bit_buf->bufsize = bufsize;
    bit_buf->buf = (unsigned char *)buf;
    BITHANDLE_MESSAGE("[Message] BitHandle init ok! Buffer size:%d\n",bufsize);

    return 0;
}

static int BitHandle_Inited(BIT_BUF *bit_buf)
{
    if(bit_buf->buf == NULL)
    {
        BITHANDLE_ERROR("[Error] BitHandle: BIT_BUF not init!\n");
        return -1;
    }
    return 0;
}

/**********************************************
 *根据bits的大小1bit 1bit的设置
 *根据mask设置1bit mask>>1
 *根据pos设置buf的第几个字节
 ***********************************************/
int BitHandle_Set(BIT_BUF *bit_buf, unsigned int value, unsigned char bits)
{
    unsigned int val_mask;

    if(bit_buf == NULL || bits == 0 || bits > 32)
    {
        BITHANDLE_ERROR("[Error] BitHandle set error,Invalid input parameters!\n");
        return -1; 
    }

    if(BitHandle_Inited(bit_buf))
        return -1;

    /*获取value的最高位的mask*/
    val_mask = 1<<(bits-1);

    bit_buf->btis += bits;

    while(bits--)
    {
        if(val_mask & value)
            bit_buf->buf[pos] |= bit_buf->mask;
        val_mask >>= 1;
        bit_buf->mask >>= 1;
        if(bit_buf->mask == 0)
        {    
            bit_buf->mask = 0x80;
            bit_buf->pos++;
        }
    }

    return 0;
}

/************************************************
 *根据bits大小 1bit 1bit的从buf获取
 *
 ************************************************/
int BitHandle_Get(BIT_BUF *bit_buf, unsigned int *value, unsigned char bits)
{
    unsigned int tmp_value;

    if(bit_buf == NULL || value == NULL || bits == 0 || bits > 32)
    {
        BITHANDLE_ERROR("[Error] BitHandle get error,Invalid input parameters!\n");
        return -1;
    }

    if(BitHandle_Inited(bit_buf))
        return -1;

    tmp_value = 0;

    while(bits--)
    {
        if(bit_buf->buf[pos] & bit_buf->mask)
            tmp_value |= 1 << (bits - 1);
        bit_buf->mask >>= 1;
    }

    *value = tmp_value;

    return 0;
}
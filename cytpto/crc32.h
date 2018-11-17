#ifndef _CRC32_H_
#define _CRC32_H_

extern int Calc_CRC32(unsigned char *buffer, unsigned int size);
extern unsigned int tvh_crc32(const unsigned char *data, int datalen, unsigned int crc);
#endif
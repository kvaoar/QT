#include "crc32.h"

CRC32::CRC32(){

}

#define CRC32_POLY   0x04C11DB7

uint32_t STM_crc_32_update(uint32_t crc, uint32_t data)
{
crc = crc ^ data;
for(uint32_t i=0;i<32;i++)
if (crc & 0x80000000)
crc = (crc << 1) ^ 0x04C11DB7;
else
crc = (crc << 1);
return(crc);
}


uint32_t CRC32::crc32_4byte( uint32_t *buf, int len)
{
    uint32_t v;
    uint32_t crc;
    crc = 0xFFFFFFFF;
    while(len >= 4) {
            v = (*buf++);
            crc = STM_crc_32_update(crc,v);
            len-=4;
    }

    return crc;

}





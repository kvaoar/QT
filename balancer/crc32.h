#ifndef CRC32_H
#define CRC32_H

#include <QString>
#include <QIODevice>

class CRC32
{

public:
    CRC32();
    uint32_t crc32_4byte(uint32_t *buf, int len);
private:

};

#endif // CRC32_H

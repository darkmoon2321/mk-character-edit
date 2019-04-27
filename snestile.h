#ifndef SNESTILE_H
#define SNESTILE_H

#include <stdint.h>
#include <vector>
#include <string>

class SNESTile
{
public:
    SNESTile();
    void readDirect(uint8_t *);
    void decompress(uint8_t *);
    std::string compress();
    uint8_t ** colorIndexedImage();
    std::string getData();
private:
    uint8_t data[0x20];
    uint16_t checksum;
    uint16_t id;
    uint8_t * address;
    std::vector <uint8_t *> usage;   //pointers to animation frames using this tile

};

#endif // SNESTILE_H

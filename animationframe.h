#ifndef ANIMATIONFRAME_H
#define ANIMATIONFRAME_H

#include "snestile.h"
#include <vector>
#include <stdint.h>
#include <QPoint>

extern uint8_t * rom_data;
uint32_t convertToLoROM(uint32_t addr);

class animationFrame
{
public:
    animationFrame();
    void set(uint32_t,uint8_t,const std::vector<SNESTile *> &);
    QPoint getTilePosition(uint16_t);
    SNESTile * getTile(uint16_t);
    uint16_t size();
    uint16_t size8();
private:
    std::vector<SNESTile *> tiles;
    std::vector<SNESTile *> tiles8;
    std::vector<int8_t> x;
    std::vector<int8_t> y;
    std::vector<int8_t> x8;
    std::vector<int8_t> y8;
    uint8_t num16;
};

#endif // ANIMATIONFRAME_H

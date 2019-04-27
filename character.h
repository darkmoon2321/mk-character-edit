#ifndef CHARACTER_H
#define CHARACTER_H

#include "snestile.h"
#include "animationframe.h"
#include "animation.h"
#include <QColor>

extern uint8_t * data;

class character
{
public:
    character(uint8_t);
    void addTile(SNESTile *);
    void addFrame(uint32_t, uint8_t);
    void addAnimation(animation *);
    void setBanks(uint8_t, uint8_t);
    std::vector<SNESTile *> getCompressedTileList();
    animationFrame * getFrame(uint16_t);
    animation * getAnimation(uint16_t);
    int getNumAnimations();
    void getPalette(QColor *);
private:
    std::vector<SNESTile *> tiles;
    animationFrame * frames[0x1000];
    std::vector<animation *> animations;
    uint16_t num_frames;
    uint32_t tile_bank;
    uint32_t frame_bank;
    uint8_t id;
};

#endif // CHARACTER_H

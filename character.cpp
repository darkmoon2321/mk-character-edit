#include "character.h"

character::character(uint8_t value)
{
    for(int i=0;i<0x1000;i++){
        frames[i] = NULL;
    }
    num_frames = 0;
    id = value;
}

void character::addTile(SNESTile * to_add){
    tiles.push_back(to_add);
}

void character::addFrame(uint32_t pointer,uint8_t frame_num){
    if(frames[frame_num]!=NULL) return;

    animationFrame * frame = new animationFrame;
    frame->set(pointer,tile_bank,tiles);
    frames[frame_num] = frame;
}

void character::setBanks(uint8_t val,uint8_t val2){
    tile_bank = val;
    frame_bank = val2;
}

void character::addAnimation(animation * anim){
    animations.push_back(anim);
}

std::vector<SNESTile *> character::getCompressedTileList(){
    return tiles;
}

animationFrame * character::getFrame(uint16_t index){
    return frames[index];
}

animation * character::getAnimation(uint16_t index){
    return animations[index];
}

int character::getNumAnimations(){
    return animations.size();
}

void character::getPalette(QColor * colors){
    uint32_t palette_pointer = (frame_bank<<16) + 0x8000;
    palette_pointer = convertToLoROM(palette_pointer);
    uint32_t color_pointer = 0xB9FB86 + (((uint32_t)id)<<7);
    color_pointer = convertToLoROM(color_pointer);
    for(int i=1;i<0x10;i++){
        uint16_t color_index = rom_data[palette_pointer + (i<<1)] + (rom_data[palette_pointer + 1 + (i<<1)]<<8);
        uint16_t color_value = rom_data[color_pointer + color_index] + (rom_data[color_pointer + color_index + 1]<<8);
        colors[i].setRed((color_value&0x1F)<<3);
        colors[i].setGreen(((color_value>>5)&0x1F)<<3);
        colors[i].setBlue(((color_value>>10)&0x1F)<<3);
    }
}

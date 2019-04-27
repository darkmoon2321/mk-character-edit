#include "animationframe.h"

animationFrame::animationFrame()
{

}

void animationFrame::set(uint32_t pointer, uint8_t tile_bank, const std::vector<SNESTile *> & compressed_tile_list){
    tiles.clear();
    x.clear();
    y.clear();

    uint8_t num_tiles = rom_data[pointer++];
    uint16_t tile_ref;
    num16 = rom_data[pointer++];
    for(int i=0;i<num16;i++){
        x.push_back(rom_data[pointer++]);
        y.push_back(rom_data[pointer++]);
        for(int j=0;j<4;j++){
            tile_ref = rom_data[pointer++];
            tile_ref += rom_data[pointer++]<<8;
            if(tile_ref&0x8000){
                SNESTile * current_tile;
                uint32_t temp_pointer = tile_ref&0xFFE0;
                temp_pointer += tile_bank<<16;
                temp_pointer += (tile_ref&0x1F)<<16;
                temp_pointer = convertToLoROM(temp_pointer);
                current_tile = new SNESTile;
                current_tile->readDirect(&rom_data[temp_pointer]);
                tiles.push_back(current_tile);
            }
            else{
                if((tile_ref/3) >= compressed_tile_list.size()){
                    tiles.push_back(compressed_tile_list.at(0));
                }
                else{
                    tiles.push_back(compressed_tile_list.at(tile_ref/3));   //tiles referenced by a 3 byte pointer in RAM, ordered by compressed size.
                }
            }
        }
    }
    for(int i=num16;i<num_tiles;i++){
        x8.push_back(rom_data[pointer++]);
        y8.push_back(rom_data[pointer++]);
        tile_ref = rom_data[pointer++];
        tile_ref += rom_data[pointer++]<<8;
        if(tile_ref&0x8000){
            SNESTile * current_tile;
            uint32_t temp_pointer = tile_ref&0xFFE0;
            temp_pointer += tile_bank<<16;
            temp_pointer += (tile_ref&0x1F)<<16;
            temp_pointer = convertToLoROM(temp_pointer);
            current_tile = new SNESTile;
            current_tile->readDirect(&rom_data[temp_pointer]);
            tiles8.push_back(current_tile);
        }
        else{
            if((tile_ref/3) >= compressed_tile_list.size()){
                tiles8.push_back(compressed_tile_list.at(0));
            }
            else{
                tiles8.push_back(compressed_tile_list.at(tile_ref/3));   //tiles referenced by a 3 byte pointer in RAM, ordered by compressed size.
            }
        }
    }
}

QPoint animationFrame::getTilePosition(uint16_t index){
    if(index >= num16){
        return QPoint(x8.at(index-num16),y8.at(index-num16));
    }
    return QPoint(x.at(index),y.at(index));
}

SNESTile * animationFrame::getTile(uint16_t index){
    if(index >= tiles.size()){
        return tiles8.at(index-tiles.size());
    }
    return tiles.at(index);
}

uint16_t animationFrame::size(){
    return tiles.size();
}

uint16_t animationFrame::size8(){
    return tiles8.size();
}

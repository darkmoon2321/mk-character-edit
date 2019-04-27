#include "snestile.h"

SNESTile::SNESTile()
{

}

void SNESTile::readDirect(uint8_t * rom_address){
    int i;
    checksum = 0;
    for(i=0;i<0x20;i++){
        data[i] = rom_address[i];
        checksum += rom_address[i];
    }
    address = rom_address;
}

void SNESTile::decompress(uint8_t * rom_address){
    int i,j;
    int read_position = 4;
    int write_position = 0;
    uint8_t control_code;
    checksum = 0;
    for(i=0;i<4;i++){
        control_code = rom_address[i];
        for(j=0;j<8;j++){
            if(control_code&0x80){
                checksum += rom_address[read_position];
                data[write_position++] = rom_address[read_position++];
            }
            else{
                data[write_position++] = 0;
            }
            control_code<<=1;
        }
    }
    address = rom_address;
}

std::string SNESTile::compress(){
    int i,j;
    uint8_t control_code;
    uint8_t data_position = 0;
    std::string result = "";
    result += (uint8_t)0;
    result += (uint8_t)0;
    result += (uint8_t)0;
    result += (uint8_t)0;
    for(i=0;i<4;i++){
        control_code = 0;
        for(j=0;j<8;j++){
            control_code<<=1;
            if(data[data_position]){
                control_code |= 1;
                result += data[data_position];
            }
            data_position++;
        }
        result[i] = control_code;
    }
    return result;
}

uint8_t ** SNESTile::colorIndexedImage(){
    uint8_t ** result = new uint8_t * [8];
    uint8_t i,j,color_index;
    for(i=0;i<8;i++){
        result[i] = new uint8_t[8];
        for(j=0;j<8;j++){
            color_index = 0;
            color_index |= (data[(i<<1)] >> (7-j))&0x01;
            color_index |= ((data[(i<<1)+1] >> (7-j))&0x01)<<1;
            color_index |= ((data[(i<<1)+0x10] >> (7-j))&0x01)<<2;
            color_index |= ((data[(i<<1)+0x11] >> (7-j))&0x01)<<3;
            result[i][j] = color_index;
        }
    }
    return result;
}

std::string SNESTile::getData(){
    std::string result = "";
    for(int i=0;i<0x20;i++){
        result += data[i];
    }
    return result;
}

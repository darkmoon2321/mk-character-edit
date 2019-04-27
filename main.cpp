#include "mainwindow.h"
#include <QApplication>
#include "snestile.h"
#include "vector"

std::vector <SNESTile *> tile_list;
std::vector <character *> characters;
//const int COMPRESSED_TILE_INFO = 0x1B9A;
uint8_t * rom_data;

char convertToASCII(char c){
     return (c<10)? c+48 : c+55;
}

std::string convertByteToHexString(uint8_t to_convert){
    int i=0;
    uint8_t letters[2];
    std::string s;

    for(i=0;i<2;i++)
    {
         letters[i]=to_convert&0xF;
         letters[i]=convertToASCII(letters[i]);
         to_convert=to_convert>>4;
    }
    for(i=1;i>=0;i--)
    {
         s+=letters[i];
    }
    return s;
}

uint32_t convertToLoROM(uint32_t addr){
    uint16_t lo = addr&0x7FFF;
    uint32_t hi = addr&0x3F0000;
    uint32_t result;
    hi>>=1;
    result = hi+lo;
    return result;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

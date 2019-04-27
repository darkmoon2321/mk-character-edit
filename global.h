#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>
#include <stdint.h>
#include "snestile.h"
#include "character.h"

extern std::vector <SNESTile *> tile_list;
extern std::vector <character *> characters;
extern uint8_t * rom_data;

uint32_t convertToLoROM(uint32_t addr);
char convertToASCII(char);
std::string convertByteToHexString(uint8_t);

#endif // GLOBAL_H

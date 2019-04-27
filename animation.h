#ifndef ANIMATION_H
#define ANIMATION_H

#include "animationframe.h"
#include <vector>
#include <stdint.h>
#include "snestile.h"

struct animationStep{
    uint16_t type;
    animationFrame * frame;
    uint16_t arg1;
    uint16_t arg2;
};

class animation
{
public:
    animation();
    void addFrame(animationFrame *);
    void addStep(uint16_t type, uint16_t word1 = 0,uint16_t word2 = 0);
    animationFrame * getFrame(uint16_t);
    uint16_t getNumSteps();
private:
    std::vector <animationStep *> steps;
    bool loop;
    uint16_t loop_location;
};

#endif // ANIMATION_H

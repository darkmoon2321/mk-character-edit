#include "animation.h"

animation::animation()
{

}

void animation::addFrame(animationFrame * frame){
    animationStep * step = new animationStep;
    step->type = 1;
    step->frame = frame;
    steps.push_back(step);
}

void animation::addStep(uint16_t type, uint16_t word1, uint16_t word2){
    animationStep * step = new animationStep;
    step->type = type;
    step->arg1 = word1;
    step->arg2 = word2;
    steps.push_back(step);
}

animationFrame * animation::getFrame(uint16_t index){
    if(steps.at(index)->type!=1){
        return NULL;
    }
    return steps.at(index)->frame;
}

uint16_t animation::getNumSteps(){
    return steps.size();
}

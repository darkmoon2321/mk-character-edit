#include "tileedititem.h"

tileEditItem::tileEditItem()
{

}

void tileEditItem::setID(uint8_t value){
    id = value;
}

uint8_t tileEditItem::getID(){
    return id;
}

void tileEditItem::mousePressEvent(QGraphicsSceneMouseEvent * event){
    emit tile_pressed(event);
}

void tileEditItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event){
    if(event->buttons() == Qt::LeftButton){
        emit tile_pressed(event);
    }
}

void tileEditItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event){
    emit tile_released();
}

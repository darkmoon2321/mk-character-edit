#include "spritetileitem.h"

SpriteTileItem::SpriteTileItem(QObject *parent) : QObject(parent)
{

}

void SpriteTileItem::setID(uint16_t value){
    id = value;
}

uint16_t SpriteTileItem::getID(){
    return id;
}

void SpriteTileItem::mousePressEvent(QGraphicsSceneMouseEvent * event){
    QGraphicsPixmapItem::mousePressEvent(event);
    setSelected(true);
    emit arrangement_pressed();
    event->ignore();
}


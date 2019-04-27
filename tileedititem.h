#ifndef TILEEDITITEM_H
#define TILEEDITITEM_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QPoint>

class tileEditItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    tileEditItem();
    void mousePressEvent(QGraphicsSceneMouseEvent *);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *);
    void setID(uint8_t);
    uint8_t getID();
signals:
    void tile_pressed(QGraphicsSceneMouseEvent *);
    void tile_released();
private:
    uint8_t id;

};

#endif // TILEEDITITEM_H

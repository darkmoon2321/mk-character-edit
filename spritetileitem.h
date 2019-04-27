#ifndef SPRITETILEITEM_H
#define SPRITETILEITEM_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>

class SpriteTileItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit SpriteTileItem(QObject *parent = 0);
    void mousePressEvent(QGraphicsSceneMouseEvent *);
    //void mouseMoveEvent(QGraphicsSceneMouseEvent *);
    //void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void setID(uint16_t);
    uint16_t getID();
signals:
    void arrangement_pressed();
    void arrangement_mouse_move();
    void arrangement_mouse_release();
private:
    uint16_t id;

public slots:
};

#endif // SPRITETILEITEM_H

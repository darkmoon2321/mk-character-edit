#ifndef PALETTESELECTOR_H
#define PALETTESELECTOR_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include "global.h"
#include "colorpicker.h"
#include "dualbutton.h"
#include <QMouseEvent>
#include <QBitmap>
#include <QColor>

class paletteSelector : public QWidget
{
    Q_OBJECT
public:
    explicit paletteSelector(QWidget *parent,int,int,QColor *);
    ~paletteSelector();
    void redraw();
    void moveEvent(QMoveEvent *event);
    void setColors(QColor *);
    QColor * getColors();
signals:
    void color_changed(uint8_t);
private slots:
    void colorPushed(QMouseEvent *);
private:
    QWidget * parent;
    QColor colors[16];
    int x;
    int y;
    dualButton buttons[16];
    bool edit_mode;
    QLabel * color_border;
    uint8_t current_color;
    QPixmap color_border_pix;
};

#endif // PALETTESELECTOR_H

#include "paletteselector.h"

paletteSelector::paletteSelector(QWidget *parent_window,int base_x,int base_y, QColor * pals) : QWidget(parent_window)
{
    parent = parent_window;
    x = base_x;
    y = base_y;
    this->lower();
    edit_mode = true;
    color_border = NULL;
    color_border_pix.load("://img/color_selector.png");
    current_color = 0;
    for(int i = 0; i< 16;i++){
        connect(&(buttons[i]),SIGNAL(dualClicked(QMouseEvent*)),this,SLOT (colorPushed(QMouseEvent *)));
        buttons[i].setParent(parent);
        colors[i] = pals[i];
        buttons[i].setGeometry(x + 5 + (i*30),y + 31,25,25);
        buttons[i].setText(QString(""));
    }
    redraw();
    color_border = new QLabel(QString("TESTING"),parent);
    color_border->setPixmap(color_border_pix);
    color_border->setGeometry(x,y + 26,35,35);
    current_color = 0;
    color_border->show();
    color_border->setMask(color_border_pix.createMaskFromColor(Qt::transparent,Qt::MaskInColor));
}


void paletteSelector::redraw(){
    unsigned int i;
    QPalette pal;
    for(i=0;i<16;i++){
        buttons[i].setAutoFillBackground(true);
        pal = buttons[i].palette();
        pal.setColor(QPalette::Button,colors[i]);
        buttons[i].setPalette(pal);
        buttons[i].update();
    }
}

void paletteSelector::setColors(QColor * pal){
    for(int i = 0;i<16;i++) colors[i] = pal[i];
    redraw();
}

void paletteSelector::colorPushed(QMouseEvent * event){
    QPushButton * b = qobject_cast<QPushButton *>(sender());
    if(event->button() == Qt::RightButton){
        edit_mode = false;
    }
    else{
        edit_mode = true;
    }
    unsigned int i;
    for(i=0;i<16;i++){
        if(b == &(buttons[i])) break;
    }
    if(i<16){
        if(edit_mode){
            if(color_border){
                color_border->setPixmap(color_border_pix);
            }
            else{
                color_border = new QLabel(QString("TESTING"),parent);
                color_border->setPixmap(color_border_pix);
            }
            if(!i){
                color_border->setGeometry(x,y + 26,35,35);
                current_color = 0;
            }
            else{
                i--;
                color_border->setGeometry(x + 40 + (i*30),y + 26,35,35);
                current_color = i;
            }
            color_border->show();
        }
        else{
            colorPicker picker;
            if(i!=0){
                picker.setDefaultColor(colors[i]);
                colors[i] = picker.exec();
            }
            redraw();
        }
        emit color_changed(current_color);
    }

}

QColor * paletteSelector::getColors(){
    return colors;
}

paletteSelector::~paletteSelector(){
    delete color_border;
}

void paletteSelector::moveEvent(QMoveEvent *event){
    int offset_x = x + event->pos().x();
    int offset_y = y + event->pos().y();

    buttons[0].move(offset_x+5,offset_y + 31);
    unsigned int i;
    for(i=1;i<16;i++){
            buttons[i].move(offset_x + 15 + (i*30),offset_y + 31);
    }
    if(!current_color){
        color_border->move(offset_x,offset_y + 26);
    }
    else{
        color_border->move(offset_x + 40 + ((current_color-1)*30),offset_y + 26);
    }
}

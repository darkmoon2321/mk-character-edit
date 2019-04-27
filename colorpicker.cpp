#include "colorpicker.h"
#include "ui_colorpicker.h"

colorPicker::colorPicker(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::colorPicker)
{
    ui->setupUi(this);
}

colorPicker::~colorPicker()
{
    delete ui;
}

void colorPicker::setDefaultColor(QColor color){
    selected_color = color;
}

QColor colorPicker::getResult(){
    return selected_color;
}

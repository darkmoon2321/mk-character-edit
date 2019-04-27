#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QDialog>
#include <QSpinBox>

namespace Ui {
class colorPicker;
}

class colorPicker : public QDialog
{
    Q_OBJECT

public:
    explicit colorPicker(QWidget *parent = 0);
    void setDefaultColor(QColor);
    ~colorPicker();
    QColor getResult();
private:
    Ui::colorPicker *ui;
    QColor selected_color;
};

#endif // COLORPICKER_H

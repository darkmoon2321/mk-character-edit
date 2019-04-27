#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "global.h"
#include "paletteselector.h"
#include "spritetileitem.h"
#include <QFileDialog>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QGraphicsSceneMouseEvent>
#include "tileedititem.h"
#include <QTransform>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionExit_triggered();

    void on_character_box_currentIndexChanged(int index);

    void on_animation_box_currentIndexChanged(int index);

    void on_frame_box_currentIndexChanged(int index);

    void on_add_animation_button_clicked();

    void on_delete_animation_button_clicked();

    void on_add_frame_button_clicked();

    void on_delete_frame_button_clicked();

    void on_tile_x_box_valueChanged(int arg1);

    void on_tile_y_box_valueChanged(int arg1);

    void on_pushButton_clicked();

    void on_delete_tile_button_clicked();

    void on_zoom_in_button_clicked();

    void on_zoom_out_button_clicked();

    void onSpriteTileClicked();

private:
    const int COMPRESSED_TILE_INFO = 0x1B9A;

    Ui::MainWindow *ui;
    uint32_t file_size;
    std::string file_location;
    paletteSelector * palSelector;
    QColor colors[16];
    std::vector <SpriteTileItem *> graphics_items;
    std::vector <uint8_t **> indexed_tiles;
    std::vector <uint8_t **> indexed_tiles8;
    std::vector <QPoint> tile_positions;
    std::vector <QPoint> tile_positions8;
    uint16_t num16;
    character * current_character;
    animation * current_animation;
    animationFrame * current_frame;
    QGraphicsScene * sprite_scene;
    QGraphicsScene * tile_scene;
    uint16_t selected_tile;
    tileEditItem * edit_tile;

    void redrawSprite();
    void getROMData();
    void readImageData();

};

#endif // MAINWINDOW_H

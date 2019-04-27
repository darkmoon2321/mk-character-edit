#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    for(int i=0;i<16;i++){
        colors[15-i] = QColor::fromRgba((i<<28) | (i<<20) | (i<<12) | (i<<4));
        colors[15-i].setAlpha(0xff);
    }
    colors[0].setAlpha(0);
    palSelector = new paletteSelector(this,10,450,colors);
    sprite_scene = new QGraphicsScene;
    tile_scene = new QGraphicsScene;
    ui->graphicsView->setScene(sprite_scene);
    ui->graphicsView_2->setScene(tile_scene);
    selected_tile = 0;
    edit_tile = new tileEditItem;
    tile_scene->addItem(edit_tile);
    edit_tile->setOffset(0,0);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete sprite_scene;
    delete tile_scene;
    delete palSelector;
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,QString(""),QString(""),tr("ROM file (*.sfc *.smc)"));
    if(!fileName.isEmpty()){
        QFile infile(fileName);
        if(!infile.open(QIODevice::ReadOnly)){
            return;
        }
        file_size = infile.size();
        QDataStream in(&infile);
        if(file_size&0x200){
            rom_data = new uint8_t[file_size - 0x200];
            in.skipRawData(0x200);
            in.readRawData((char *)rom_data,file_size - 0x200);
        }
        else{
            rom_data = new uint8_t[file_size];
            in.readRawData((char *)rom_data,file_size);
        }
        getROMData();
        file_location = fileName.toStdString();
        unsigned int i,j;
        for(i=file_location.length() - 1; i!=0;i--){
            if(file_location[i] == '\\' || file_location[i] == '/') break;
        }
        if(file_location[i] == '\\' || file_location[i] == '/'){
            std::string temp_string = "";
            for(j=0;j<i;j++){
                temp_string += file_location[j];
            }
            file_location = temp_string;
        }
    }
}

void MainWindow::on_actionSave_triggered()
{
    std::string write_data = "";
    for(int i=0;i<4;i++) write_data+=((SNESTile *)(current_frame->getTile((selected_tile<<2) + i)))->getData();

    QString filename = QFileDialog::getSaveFileName(this,QString(""),QString(""),tr("ROM file (*.nes)"));
    if(!filename.isNull()){
        QFile outfile(filename);
        if(!outfile.open(QIODevice::WriteOnly)){
            QMessageBox::critical(this,tr("Error!"),tr("Could not create file."));
            return;
        }
        QDataStream out(&outfile);
        for(unsigned int i=0;i<write_data.length();i++){
            out<<(uint8_t)write_data[i];
        }
    }
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::getROMData(){
    int i,j;
    int character_number = 0;
    uint32_t character_pointer,old_pointer,animation_frame,animation_pointer,animation_frame_list,animation_frame_bank;
    uint16_t frame_number;
    old_pointer = 0xffffff;
    character_pointer = rom_data[COMPRESSED_TILE_INFO] + (rom_data[COMPRESSED_TILE_INFO + 1]<<8) + 0xAE0000;
    character_pointer = convertToLoROM(character_pointer);
    while(character_pointer < old_pointer){
        uint32_t tile_data;
        uint32_t size_data;
        current_character = new character(character_number);
        tile_data = rom_data[character_pointer-4] | (rom_data[character_pointer-3]<<8);
        tile_data <<= 5;
        //tile_data |= 0x8000;
        tile_data = (tile_data&0x7FFF) | 0x8000;
        tile_data |= (rom_data[character_pointer - 6]) << 16;
        tile_data += (rom_data[character_pointer - 3] >> 2) << 16;
        //tile_data = convertToLoROM(tile_data);
        uint16_t num_sizes[0x1B];
        size_data = rom_data[character_pointer - 0xC] | (rom_data[character_pointer - 0xB]<<8) | 0xAE0000;
        size_data = convertToLoROM(size_data);
        int DEBUG_COUNT = 0;
        for(i=0;i<0x1B;i++){
            num_sizes[i] = rom_data[size_data] + (rom_data[size_data+1]<<8);
            size_data+=2;
            for(j=0;j<num_sizes[i];j++){
                if(DEBUG_COUNT == (0x1B06/3)){
                    DEBUG_COUNT = 0;
                }
                SNESTile * current_tile = new SNESTile;
                current_tile->decompress(&(rom_data[convertToLoROM(tile_data)]));
                std::string debug_string = current_tile->compress();
                if(debug_string.length() != (i+5)){
                    i++;
                    i--;
                }
                tile_data += (i+5);
                if(!(tile_data&0x8000)){    //Handle errors at bank boundaries
                    tile_data = (tile_data&0xFF0000)|0x8000;
                    j--;
                    continue;
                }
                tile_list.push_back(current_tile);
                current_character->addTile(current_tile);
                DEBUG_COUNT++;
            }
        }
        animation_pointer = rom_data[character_pointer] + (rom_data[character_pointer+1]<<8);

        animation_frame_list = rom_data[character_pointer - 0xa] + (rom_data[character_pointer - 9]<<8) + 0xAE0000;
        animation_frame_list = convertToLoROM(animation_frame_list);
        animation_frame_bank = rom_data[character_pointer - 8]<<16;
        current_character->setBanks(rom_data[character_pointer - 6],rom_data[character_pointer - 8]);
        i=0;
        while(animation_pointer){
            animation_pointer += 0xAE0000;
            animation_pointer = convertToLoROM(animation_pointer);
            j=0;
            frame_number = rom_data[animation_pointer + (j<<1)] + (rom_data[animation_pointer+1 + (j<<1)]<<8);
            current_animation = new animation;
            while(frame_number!=0xffff && frame_number!=0){
                uint16_t stepArg1,stepArg2;
                if(frame_number>=0xfff6){
                    switch(frame_number){
                    case 0xfff6:
                    case 0xfff8:
                    case 0xfff9:
                    case 0xfffa:
                    case 0xfffc:
                        j++;
                        stepArg1 = rom_data[animation_pointer + (j<<1)] + (rom_data[animation_pointer + 1 + (j<<1)]<<8);
                        current_animation->addStep(frame_number,stepArg1);
                        break;
                    case 0xfff7:
                    case 0xfffd:
                        j++;
                        stepArg1 = rom_data[animation_pointer + (j<<1)] + (rom_data[animation_pointer + 1 + (j<<1)]<<8);
                        j++;
                        stepArg2 = rom_data[animation_pointer + (j<<1)] + (rom_data[animation_pointer + 1 + (j<<1)]<<8);
                        current_animation->addStep(frame_number,stepArg1,stepArg2);
                        break;
                    case 0xfffb:
                    case 0xfffe:
                        current_animation->addStep(frame_number);
                        break;
                    }
                }
                else{
                    frame_number--;
                    animation_frame = animation_frame_bank + rom_data[animation_frame_list + (frame_number<<1)] + (rom_data[animation_frame_list + (frame_number<<1) + 1]<<8);
                    animation_frame = convertToLoROM(animation_frame);
                    current_character->addFrame(animation_frame,frame_number);
                    current_animation->addFrame(current_character->getFrame(frame_number));
                }
                j++;
                frame_number = rom_data[animation_pointer + (j<<1)] + (rom_data[animation_pointer + 1 + (j<<1)]<<8);
            }
            current_character->addAnimation(current_animation);
            i++;
            animation_pointer = rom_data[character_pointer + (i<<1)] + (rom_data[character_pointer+1 + (i<<1)]<<8);
        }

        characters.push_back(current_character);
        ui->character_box->addItem(QString(convertByteToHexString(character_number).c_str()));
        old_pointer = character_pointer;
        character_number++;
        character_pointer = rom_data[COMPRESSED_TILE_INFO + (character_number<<1)] + (rom_data[COMPRESSED_TILE_INFO + (character_number<<1) + 1]<<8) + 0xAE0000;
        character_pointer = convertToLoROM(character_pointer);
    }
    ui->character_box->setCurrentIndex(0);
}

void MainWindow::on_character_box_currentIndexChanged(int index)
{
    if(characters.size()==0) return;
    if(index == -1){
        current_character = characters.at(0);
    }
    else{
        current_character = characters.at(index);
    }
    for(int i=0;i<current_character->getNumAnimations();i++){
        ui->animation_box->addItem(QString(convertByteToHexString(i).c_str()));
    }
    current_character->getPalette(colors);
    ui->animation_box->setCurrentIndex(0);
}

void MainWindow::on_animation_box_currentIndexChanged(int index)
{
    if(!current_character) return;
    if(index == -1){
        current_animation = current_character->getAnimation(0);
    }
    else{
        current_animation = current_character->getAnimation(index);
    }
    for(int i=0;i<current_animation->getNumSteps();i++){
        ui->frame_box->addItem(QString(convertByteToHexString(i).c_str()));
    }
    ui->frame_box->setCurrentIndex(0);
}


void MainWindow::on_frame_box_currentIndexChanged(int index)
{
    if(!current_character || !current_animation) return;
    if(index == -1){
        current_frame = current_animation->getFrame(0);
    }
    else{
        current_frame = current_animation->getFrame(index);
    }
    readImageData();
    palSelector->setColors(colors);
    redrawSprite();
}

void MainWindow::on_add_animation_button_clicked()
{

}


void MainWindow::on_delete_animation_button_clicked()
{

}


void MainWindow::on_add_frame_button_clicked()
{

}

void MainWindow::on_delete_frame_button_clicked()
{

}

void MainWindow::on_tile_x_box_valueChanged(int arg1)
{

}

void MainWindow::on_tile_y_box_valueChanged(int arg1)
{

}

void MainWindow::on_pushButton_clicked()    //new tile button
{

}

void MainWindow::on_delete_tile_button_clicked()
{

}

void MainWindow::readImageData(){
    int i,j;
    for(i=0;i<graphics_items.size();i++){
        delete graphics_items.at(i);
    }
    graphics_items.clear();
    for(i=0;i<indexed_tiles.size();i++){
        for(j=0;j<8;j++) delete indexed_tiles.at(i)[j];
        delete indexed_tiles.at(i);
    }
    for(i=0;i<indexed_tiles8.size();i++){
        for(j=0;j<8;j++) delete indexed_tiles8.at(i)[j];
        delete indexed_tiles8.at(i);
    }
    indexed_tiles.clear();
    indexed_tiles8.clear();
    tile_positions.clear();
    tile_positions8.clear();
    uint8_t character_num = ui->character_box->currentIndex();
    uint16_t animation_num = ui->animation_box->currentIndex();
    uint16_t frame_num = ui->frame_box->currentIndex();
    graphics_items.clear();

    current_character = characters.at(character_num);
    current_animation = current_character->getAnimation(animation_num);
    current_frame = current_animation->getFrame(frame_num);

    SNESTile * current_tile;
    uint8_t ** indexed_tile;
    num16 = current_frame->size()>>2;
    for(i=0;i<current_frame->size();i++){
        current_tile = current_frame->getTile(i);
        indexed_tile = current_tile->colorIndexedImage();
        indexed_tiles.push_back(indexed_tile);
        if((i&3)==0){
            tile_positions.push_back(current_frame->getTilePosition(i>>2));
            graphics_items.push_back(new SpriteTileItem());
            sprite_scene->addItem(graphics_items.at(i>>2));
            ((SpriteTileItem *)graphics_items.at(i>>2))->setID(i>>2);
            connect(((SpriteTileItem *)graphics_items.at(i>>2)),\
                    SIGNAL(arrangement_pressed()),this,\
                    SLOT(onSpriteTileClicked()));
        }
    }
    for(i=0;i<current_frame->size8();i++){
        current_tile = current_frame->getTile(current_frame->size()+i);
        indexed_tile = current_tile->colorIndexedImage();
        indexed_tiles8.push_back(indexed_tile);
        tile_positions8.push_back(current_frame->getTilePosition(num16 + i));
        graphics_items.push_back(new SpriteTileItem());
        sprite_scene->addItem(graphics_items.at(num16+i));
        ((SpriteTileItem *)graphics_items.at(num16+i))->setID(num16+i);
    }
}

void MainWindow::redrawSprite(){
    QImage current_tile(16,16,QImage::Format_ARGB32);
    int i,j,k;

    QRgb * edit_line;
    for(i=0;i<indexed_tiles.size();i++){
        for(j=0;j<8;j++){
            edit_line = (QRgb *)current_tile.scanLine(j + ((i&2)<<2));
            for(k=0;k<8;k++){
                edit_line[k + ((i&1)<<3)] = colors[((uint8_t **)indexed_tiles.at(i))[j][k]].rgba();
            }
        }
        if((i&3)==3){
            ((SpriteTileItem *)graphics_items.at(i>>2))->setPixmap(QPixmap::fromImage(current_tile));
            int8_t temp_x = tile_positions.at(i>>2).x()&0xff;
            int8_t temp_y = tile_positions.at(i>>2).y()&0xff;
            //temp_x = (temp_x - 0x80)^0xFF80;
            //temp_y = (temp_y - 0x80)^0xFF80;
            ((SpriteTileItem *)graphics_items.at(i>>2))->setOffset(temp_x,temp_y);
        }
    }
    QImage current_tile8(8,8,QImage::Format_ARGB32);
    for(i=0;i<indexed_tiles8.size();i++){
        for(j=0;j<8;j++){
            edit_line = (QRgb *)current_tile8.scanLine(j);
            for(k=0;k<8;k++){
                edit_line[k] = colors[((uint8_t **)indexed_tiles8.at(i))[j][k]].rgba();
            }
        }
        ((SpriteTileItem *)graphics_items.at(num16+i))->setPixmap(QPixmap::fromImage(current_tile8));
        int8_t temp_x = tile_positions8.at(i).x()&0xff;
        int8_t temp_y = tile_positions8.at(i).y()&0xff;
        //temp_x = (temp_x - 0x80)^0xFF80;
        //temp_y = (temp_y - 0x80)^0xFF80;
        ((SpriteTileItem *)graphics_items.at(num16+i))->setOffset(temp_x,temp_y);
    }
}

void MainWindow::on_zoom_in_button_clicked()
{
    //arrangement_released();
    QMatrix stretch= ui->graphicsView->matrix();
    if(stretch.m11() < 8){
        ui->graphicsView->scale(2,2);
    }
}

void MainWindow::on_zoom_out_button_clicked()
{
    //arrangement_released();
    QMatrix stretch= ui->graphicsView->matrix();
    if(stretch.m11() > 1){
        ui->graphicsView->scale(0.5,0.5);
    }
}

void MainWindow::onSpriteTileClicked(){
    SpriteTileItem * selection = qobject_cast<SpriteTileItem *>(sender());
    uint16_t tile_id = selection->getID();
    selected_tile = tile_id;
    QRgb * edit_line;
    QTransform transform;
    transform.translate(0,0);
    transform.rotate(0);
    int i,j,k;
    if(tile_id >= num16){
        i=0;
        QImage current_tile8(8,8,QImage::Format_ARGB32);
        for(j=0;j<8;j++){
            edit_line = (QRgb *)current_tile8.scanLine(j);
            for(k=0;k<8;k++){
                edit_line[k] = colors[((uint8_t **)indexed_tiles8.at(tile_id - num16))[j][k]].rgba();
            }
        }
        edit_tile->setPixmap(QPixmap::fromImage(current_tile8));
        transform.scale(12,12);
    }
    else{
        QImage current_tile(16,16,QImage::Format_ARGB32);
        for(i=0;i<4;i++){
            for(j=0;j<8;j++){
                edit_line = (QRgb *)current_tile.scanLine(j + ((i&2)<<2));
                for(k=0;k<8;k++){
                    edit_line[k + ((i&1)<<3)] = colors[((uint8_t **)indexed_tiles.at((tile_id<<2) + i))[j][k]].rgba();
                }
            }
        }
        edit_tile->setPixmap(QPixmap::fromImage(current_tile));
        transform.scale(6,6);
    }
    ui->graphicsView_2->setTransform(transform);
}

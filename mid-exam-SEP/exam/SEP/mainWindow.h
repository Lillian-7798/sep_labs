#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#endif // MAINWINDOW_H

#include <QWidget>
#include "battlefield.h"
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <fstream>
#include <iostream>
#include <QPushButton>
using namespace std;
class MainWindow : public QWidget
{
    Q_OBJECT
private:
    BattleField battle_field;
    QColor enemy_color;
    QColor safe_color;
    QColor path_color;
    QAction *loadFile;
    bool isfind=false;
    void outpath();
    QPushButton *load = new QPushButton("Load File",this);
public:
    MainWindow(QWidget *parent = nullptr);
    // use data in the file to init the battle field
    void load_data();

protected:
    // draw battle field
    void paintEvent(QPaintEvent *event) override;
    // listen to the key press event
    void keyPressEvent(QKeyEvent *event) override;

};

#ifndef SIMULATEMAPWINDOW_H
#define SIMULATEMAPWINDOW_H

#include <QWidget>
#include <vector>
#include <iostream>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "map.h"
#include "simulatemap.h"

namespace Ui {
class SimulateMapWindow;
}

class SimulateMapWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SimulateMapWindow(Map *map, QWidget *parent = 0);
    ~SimulateMapWindow();

private slots:
    void on_simulateButton_clicked();

    void on_lidarButton_clicked();

    void on_noneButton_clicked();

private:
    Ui::SimulateMapWindow *ui;
    Map *map;
    SimulateMap *simMap;
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent * event);
    Qt::GlobalColor getColorById(int id);
    int objectx = 0;
    int objecty = 0;
    int drawWidth = 430;
    int drawHeight = 460;
    int selected = 3;
    bool mousePressed = true;
};

#endif // SIMULATEMAPWINDOW_H

#ifndef VIEWCHANGEBUTTON_H
#define VIEWCHANGEBUTTON_H

#include <QWidget>
#include<QMouseEvent>
#include<QPaintEvent>
#include<QVector2D>

enum Direction {
    TOP=0,
    BOTTOM=1,
    LEFT=2,
    RIGHT=3
};

class ViewChangeButton:public QWidget
{
    Q_OBJECT
public:
    ViewChangeButton(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    bool pointInTriangle(QVector2D A, QVector2D B, QVector2D C, QVector2D P);
    void setDirection(Direction direction);
signals:
    void selectDirection(Direction dir);
private:
    Direction mDefaultDirection = TOP;
};

#endif // VIEWCHANGEBUTTON_H

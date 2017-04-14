#ifndef MYPICWIDGET_H
#define MYPICWIDGET_H

#include "usda.h"

class myPicWidget : public QGraphicsView
{
    Q_OBJECT
public:
    explicit myPicWidget(QWidget *parent = 0);
    ~myPicWidget();

public slots:
    // Called when the tree path is changed
    int on_reload();
    // Called to load a new image
    void on_updateWindow();
    // Called when the view angle is changed
    void on_updateAngle(float theta);

    void on_initWindow();


private:
    QGraphicsScene *scene;
    // Big items are allocated in heap
    QPixmap *pixmap;
    QGraphicsPixmapItem *item;
    QStringList picList;
    QDir picDir;
    int thisManyPics;
    float delta;
    int picIndex;
};

#endif // MYPICWIDGET_H

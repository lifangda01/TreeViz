#include "mypicwidget.h"

myPicWidget::myPicWidget(QWidget *parent) : QGraphicsView(parent)
{
    scene = new QGraphicsScene(this);
    pixmap = new QPixmap;
    item = new QGraphicsPixmapItem;
    on_reload();
    on_initWindow();
}

myPicWidget::~myPicWidget()
{}

int myPicWidget::on_reload()
{
    picDir = QDir(picPath);
    picDir.setSorting(QDir::Name);
    picList.clear();
    picList = picDir.entryList((QStringList()<<"*.png"<<"*.jpeg"<<"*.bmp"<<"*.jpg"<<"*.gif"));
    if (picList.empty())
    {
        qCritical("No tree image found!");
        QMessageBox::critical(this, tr("No Image Found"),
                              tr("Cannot find tree image files."),
                              QMessageBox::Ok);
        scene->clear();
        this->setScene(scene);
        return ERROR;
    }

    //    QStringListIterator it(picList);
    //    while (it.hasNext())
    //        qDebug() << it.next();

    thisManyPics = picList.size();
    delta = 360.0f / (float)thisManyPics;
    picIndex = 0;
    this->on_updateWindow();
    return OK;
}

void myPicWidget::on_updateWindow()
{
    if (picList.empty()) return;

    // Fixed widget width & height, but scale scene with image ratio
    pixmap->load(picDir.path()+ "/" +picList[picIndex]);
    float asRatio = (float)pixmap->height()/pixmap->width();

    // Deal with initially small widget width value
    int w = this->width();
    w = w < 300 ? 300 : this->width();
    int h = w*asRatio;

    scene->addPixmap(pixmap->scaled(QSize(w, h),Qt::KeepAspectRatio));
    scene->setSceneRect(0,0,w + 10, h + 10);

    this->setScene(scene);
//    qDebug() << "widget:" << this->width() << this->height();
//    qDebug() << "scene:" << scene->width() << scene->height();
//    qDebug() << "pixmap:" << pixmap->width() << pixmap->height();

}

// theta is in degrees
void myPicWidget::on_updateAngle(float theta)
{
    int prevPicIndex = picIndex;
    picIndex = ((int)(theta / delta)) % thisManyPics;
    if (picIndex < 0) picIndex = thisManyPics + picIndex;
//    qDebug("theta = %f, picIndex = %d", theta, picIndex);
    if (prevPicIndex != picIndex &&
            picIndex >= 0 &&
            picIndex < thisManyPics)
        this->on_updateWindow();
}

void myPicWidget::on_initWindow()
{
    this->on_updateWindow();
}

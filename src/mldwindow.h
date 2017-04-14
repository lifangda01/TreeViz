#ifndef MLDWINDOW_H
#define MLDWINDOW_H

#include <QDialog>
#include "usda.h"

namespace Ui {
class mldwindow;
}

class mldwindow : public QDialog
{
    Q_OBJECT

public:
    explicit mldwindow(QWidget *parent = 0);
    ~mldwindow();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::mldwindow *ui;
};

#endif // MLDWINDOW_H

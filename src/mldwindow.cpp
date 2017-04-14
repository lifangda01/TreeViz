#include "mldwindow.h"
#include "ui_mldwindow.h"

mldwindow::mldwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mldwindow)
{
    ui->setupUi(this);
    ui->sb_md1->setValue(mld1);
    ui->sb_md2->setValue(mld2);
    ui->sb_md3->setValue(mld3);
}

mldwindow::~mldwindow()
{
    delete ui;
}

void mldwindow::on_buttonBox_accepted()
{
    mld1 = ui->sb_md1->value();
    mld2 = ui->sb_md2->value();
    mld3 = ui->sb_md3->value();
}

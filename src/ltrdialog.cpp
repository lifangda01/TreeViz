#include "usda.h"
#include "ltrdialog.h"
#include "ui_ltrdialog.h"

LTRDialog::LTRDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LTRDialog)
{
    ui->setupUi(this);
    ui->hs_ltr->setRange(0, 40);
    ui->hs_ltr->setSingleStep(1);
    ui->hs_ltr->setValue((int)(dLTratio / 0.05f ));
    ui->lb_ltr->setText(QString::number(dLTratio, 'f', 2));
}

LTRDialog::~LTRDialog()
{
    delete ui;
}

void LTRDialog::on_hs_ltr_sliderMoved(int position)
{
    float ltr = (float) position / 20.0f;
    ui->lb_ltr->setText(QString::number(ltr, 'f', 2));
}

void LTRDialog::on_buttonBox_accepted()
{
    dLTratio = (float) ui->hs_ltr->value() / 20.0f;
    emit signal_updateLTR();
    qDebug() << "new dLTratio:" << dLTratio;
}

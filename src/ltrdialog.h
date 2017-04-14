#ifndef LTRDIALOG_H
#define LTRDIALOG_H

#include <QDialog>

namespace Ui {
class LTRDialog;
}

class LTRDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LTRDialog(QWidget *parent = 0);
    ~LTRDialog();

signals:
    void signal_updateLTR();

private slots:
    void on_hs_ltr_sliderMoved(int position);

    void on_buttonBox_accepted();

private:
    Ui::LTRDialog *ui;
};

#endif // LTRDIALOG_H

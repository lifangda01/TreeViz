#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "mldwindow.h"
#include "ltrdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void on_updateInfo1();
    void on_updateInfo2();
    void on_updateAngle();
    void on_updateLegends();

signals:
	void cutBranches1();
    void cutBranches2();
	void updateFrame1();
    void updateFrame2();
    void reset1();
	void reset2();
	void showPicWindow();

protected:
	void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;
//	picWindow *childPicWindow;
    mldwindow *childMLDWindow;
    LTRDialog *childLTRDialog;

private slots:
	void showPic();
	void openDir();
    void on_pb_cut1_clicked();
    void on_pb_cut2_clicked();
    void on_pb_reset1_clicked();
    void on_pb_reset2_clicked();
    void on_cb_analyze_stateChanged(int arg1);
    void on_cb_analyze2_stateChanged(int arg1);
    void on_sb_diameter_low_valueChanged(double arg1_cm);
    void on_sb_diameter_high_valueChanged(double arg1_cm);
    void on_sb_angle_low_valueChanged(double arg1);
    void on_sb_angle_high_valueChanged(double arg1);
    void on_sb_ratio_low_valueChanged(double arg1);
    void on_sb_ratio_high_valueChanged(double arg1);
    void on_pb_select_clicked();
    void on_cb_diameter_stateChanged(int arg1);
    void on_cb_angle_stateChanged(int arg1);
    void on_cb_ratio_stateChanged(int arg1);
    void on_cb_severity_stateChanged(int arg1);
    void on_sb_severity_valueChanged(double arg1);
    void on_co_approach_currentIndexChanged(int index);
    void on_pb_cutCur1_clicked();
    void on_pb_cutCur2_clicked();

    void on_cb_hlrank_clicked();

    void on_cb_hlrank2_clicked();

    void on_cb_and_or_clicked(bool checked);

    void on_sb_angle_low_editingFinished();

    void on_sb_angle_high_editingFinished();

    void on_sb_ratio_low_editingFinished();

    void on_sb_ratio_high_editingFinished();

private:
    Ui::MainWindow *ui;
	QMenu *viewMenu;
	QMenu *fileMenu;
    QMenu *editMenu;
	QAction *showPicAct;
    QAction *showLTRAct;
    QAction *showMLDAct;
	QAction *closeAct;
	QAction *openAct;
	QWidget *myCentralWidget;
	void connectSignals();
	void createActions();
	void createMenus();
	void createLayouts();
    void createLegends();
    void updateSeverityAndStyle(QLabel *label, float dLTRatio, float severity);
};

#endif // MAINWINDOW_H

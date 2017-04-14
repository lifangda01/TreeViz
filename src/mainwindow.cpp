#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

#include "usda.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createLayouts();
    this->setWindowIcon(QIcon("usda.xpm"));
    childMLDWindow = new mldwindow(this);
    childLTRDialog = new LTRDialog(this);
    connectSignals();
    createActions();
    createMenus();
    createLegends();
    qDebug("MainWindow initialization finished.");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createLayouts()
{
    this->setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    this->setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    /*Force resizing mainwindow so that we can develop two tabbed docks simoutaneously in designer.*/
}

void MainWindow::createActions()
{
//    showPicAct = new QAction(tr("Show Image"), this);
//    showPicAct->setStatusTip(tr("Show image"));
//    connect(showPicAct, SIGNAL(triggered()), childPicWindow, SLOT(show()));

    showMLDAct = new QAction(tr("Edit MLD Parameters"), this);
    showMLDAct->setStatusTip(tr("Edit MLD Parameters"));
    connect(showMLDAct, SIGNAL(triggered()), childMLDWindow, SLOT(exec()));

    showLTRAct = new QAction(tr("Edit Desired LTR"), this);
    showLTRAct->setStatusTip(tr("Edit Desired LTR"));
    connect(showLTRAct, SIGNAL(triggered()), childLTRDialog, SLOT(exec()));

    closeAct = new QAction(tr("Close"), this);
    closeAct->setShortcuts(QKeySequence::Close);
    connect(closeAct, SIGNAL(triggered()), this, SLOT(close()));

    openAct = new QAction(tr("Open Directory"), this);
    openAct->setShortcuts(QKeySequence::Open);
    connect(openAct, SIGNAL(triggered()), this, SLOT(openDir()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(closeAct);

    editMenu = menuBar()->addMenu(tr("Edit"));
    editMenu->addAction(showMLDAct);    
    editMenu->addAction(showLTRAct);

    //    viewMenu = menuBar()->addMenu(tr("View"));
    //    viewMenu->addAction(showPicAct);
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.addAction(openAct);
    //    menu.addAction(showPicAct);
    menu.addAction(closeAct);
    menu.exec(event->globalPos());
}

void MainWindow::openDir()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    // "/home/fangda/Datasets/Trees",
                                                    QDir::homePath(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if (dir == NULL)
        return;
    dir.append("/");
    /* Check if directory is valid! */
    if (QDir(dir+"BranchCircles").exists() &&
            QDir(dir+"TrunkCircles").exists() &&
            QFile(dir+objName).exists())
    {
        qDebug()<< "New treepath is " <<dir;
        /* Ubuntu uses utf8 encoding for strings */
        treePath = dir.toUtf8().constData();

        /* Load new tree images */
        picPath = dir + "Pictures";
        int loadError = 0;
        loadError += ui->picview1->on_reload();
        loadError += ui->picview2->on_reload();
        if (loadError == 0)
            QMessageBox::critical(this, tr("No Image Found"),
                                  tr("Cannot find tree image files."),
                                  QMessageBox::Ok);

        /* Reload model and all parameters */
        mainInit();
        ui->mesh1->setupNewModel(dir, QString("tree.obj"));
        ui->mesh2->setupNewModel(dir, QString("tree.obj"));
        this->connectSignals();
    }
    else
    {
        QMessageBox::critical(this, tr("Invalid Directory"),
                              tr("Make sure "\
                                 "BranchCircles, TrunkCircles and tree.obj are "\
                                 "present in directory."),
                              QMessageBox::Ok);
    }
}

void MainWindow::showPic()
{
    emit showPicWindow();
}

void MainWindow::on_updateInfo1()
{
    updateCurrentInfo(MANUAL);
    float radius = currRadius;
    float angle = currAngle;
    float ratio = currRatio;
    if (radius < 0) ui->lb_diameter1->setText("--");
    else ui->lb_diameter1->setText(QString::number(m2cm(radius)*2, 'f', 3));
    if (angle < 0) ui->lb_angle1->setText("--");
    else ui->lb_angle1->setText(QString::number(angle, 'f', 3));
    if (ratio < 0) ui->lb_ratio1->setText("--");
    else ui->lb_ratio1->setText(QString::number(ratio, 'f', 3));
}

void MainWindow::on_updateInfo2()
{
    updateCurrentInfo(AUTO);
    float radius = currRadius2;
    float angle = currAngle2;
    float ratio = currRatio2;

    qDebug() << currIndex2 << radius*2 << angle << ratio;
    if (radius < 0) ui->lb_diameter2->setText("--");
    else ui->lb_diameter2->setText(QString::number(m2cm(radius)*2, 'f', 3));
    if (angle < 0) ui->lb_angle2->setText("--");
    else ui->lb_angle2->setText(QString::number(angle, 'f', 3));
    if (ratio < 0) ui->lb_ratio2->setText("--");
    else ui->lb_ratio2->setText(QString::number(ratio, 'f', 3));
}

// Also called when reset
void MainWindow::connectSignals()
{
    // mesh1 connections
    ui->mesh1->setupSelection(&selected, &deleted, &currIndex, &analyze, &highlight);
    ui->mesh1->myModel = &model;
    ui->mesh1->showModel = 0;
    connect(ui->mesh1, SIGNAL(signal_updateInfo(void)),
            this, SLOT(on_updateInfo1(void)));
    connect(ui->mesh1, SIGNAL(signal_updateAngle(void)),
            this, SLOT(on_updateAngle(void)));
    // trigger the picture to be shown
    ui->picview1->on_updateAngle(ui->mesh1->yRot);

    // mesh2 connections
    ui->mesh2->setupSelection(&selected2, &deleted2, &currIndex2, &analyze2, &highlight2);
    ui->mesh2->myModel = &model;
    ui->mesh2->showModel = 0;
    connect(ui->mesh2, SIGNAL(signal_updateInfo(void)),
            this, SLOT(on_updateInfo2(void)));
    connect(ui->mesh2, SIGNAL(signal_updateAngle(void)),
            this, SLOT(on_updateAngle(void)));

    // trigger the picture to be shown
    ui->picview2->on_updateAngle(ui->mesh2->yRot);

    // Update legends when well pruned ltr is changed
    connect(this->childLTRDialog, SIGNAL(signal_updateLTR(void)),
            this, SLOT(on_updateLegends(void)));

    // Auto pruning parameter configurations
    // Default for spinbox is 0
    ui->sb_diameter_high->setValue(m2cm(MAX_DIAMETER));
    ui->sb_diameter_high->setSingleStep(0.2);
    ui->sb_diameter_high->setRange(m2cm(MIN_DIAMETER),m2cm(MAX_DIAMETER));
    ui->sb_diameter_low->setValue(m2cm(MIN_DIAMETER));
    ui->sb_diameter_low->setSingleStep(0.2);
    ui->sb_diameter_low->setRange(m2cm(MIN_DIAMETER),m2cm(MAX_DIAMETER));

    ui->sb_angle_high->setRange(MIN_ANGLE,MAX_ANGLE);
    ui->sb_angle_high->setValue(MAX_ANGLE);
    ui->sb_angle_high->setDecimals(1);
    ui->sb_angle_low->setRange(MIN_ANGLE,MAX_ANGLE);
    ui->sb_angle_low->setValue(MIN_ANGLE);
    ui->sb_angle_low->setDecimals(1);

    ui->sb_ratio_high->setValue(MAX_RATIO);
    ui->sb_ratio_high->setSingleStep(0.01);
    ui->sb_ratio_high->setRange(MIN_RATIO,MAX_RATIO);
    ui->sb_ratio_low->setValue(MIN_RATIO);
    ui->sb_ratio_low->setSingleStep(0.01);
    ui->sb_ratio_low->setRange(MIN_RATIO,MAX_RATIO);

    ui->sb_severity->setRange(MIN_SEVER,MAX_SEVER);
    ui->sb_severity->setSingleStep(0.05);
    ui->sb_severity->setValue(DEF_SEVER);

//    ui->sb_dratio1->setRange(MIN_SEVER,MAX_SEVER);
//    ui->sb_dratio1->setSingleStep(0.05);
//    ui->sb_dratio1->setValue(DEF_SEVER);


    // Reset the current severity
    updateSeverityAndStyle(ui->lb_ltratio1, dLTratio, getCurrentSeverity(deleted));
    updateSeverityAndStyle(ui->lb_ltratio2, dLTratio, getCurrentSeverity(deleted2));

    // Uncheck all checkboxes
    ui->cb_analyze->setChecked(false);
    ui->cb_analyze2->setChecked(false);
    ui->cb_angle->setChecked(false);
    ui->cb_diameter->setChecked(false);
    ui->cb_ratio->setChecked(false);
    ui->cb_severity->setChecked(false);
    ui->sb_diameter_high->setEnabled(false);
    ui->sb_diameter_low->setEnabled(false);
    ui->sb_angle_high->setEnabled(false);
    ui->sb_angle_low->setEnabled(false);
    ui->sb_ratio_high->setEnabled(false);
    ui->sb_ratio_low->setEnabled(false);
    ui->sb_severity->setEnabled(false);
    ui->co_approach->setEnabled(false);

    // Clear current info
    ui->lb_diameter1->setText("--");
    ui->lb_angle1->setText("--");
    ui->lb_ratio1->setText("--");
    ui->lb_diameter2->setText("--");
    ui->lb_angle2->setText("--");
    ui->lb_ratio2->setText("--");
//    ui->lb_mar2->setText("--");

}

void MainWindow::createLegends()
{
    // FIXME
    ui->lb_sevhigh1->setText("LTR > " + QString::number(dLTratio, 'f', 2));
    ui->lb_sevmed1->setText(QString::number(dLTratio, 'f', 2) + " > LTR \n"
                            "> " + QString::number(dLTratio*0.6, 'f', 2));
    ui->lb_sevlow1->setText(QString::number(dLTratio*0.6, 'f', 2) + " > LTR");
    ui->lb_sevhigh2->setText("LTR > " + QString::number(dLTratio, 'f', 2));
    ui->lb_sevmed2->setText(QString::number(dLTratio, 'f', 2) + " > LTR \n"
                            "> " + QString::number(dLTratio*0.6, 'f', 2));
    ui->lb_sevlow2->setText(QString::number(dLTratio*0.6, 'f', 2) + " > LTR");

    QString cssL = "color: rgba(%1);\
            text-align: center;";
    //    ui->lb_curBr1->setStyleSheet(cssL.arg("0,255,0,0.7"));
    //    ui->lb_selbr1->setStyleSheet(cssL.arg("255,0,0,0.7"));
    ui->lb_sevhigh1->setStyleSheet(cssL.arg("255,0,0,0.9"));
    ui->lb_sevmed1->setStyleSheet(cssL.arg("0,255,0,0.9"));
    ui->lb_sevlow1->setStyleSheet(cssL.arg("249,198,14,0.9"));

    //    ui->lb_curBr2->setStyleSheet(cssL.arg("0,255,0,0.7"));
    //    ui->lb_selBr2->setStyleSheet(cssL.arg("255,0,0,0.7"));
    //    ui->lb_cross->setStyleSheet(cssL.arg("255,255,0,0.7"));
    ui->lb_sevhigh2->setStyleSheet(cssL.arg("255,0,0,0.9"));
    ui->lb_sevmed2->setStyleSheet(cssL.arg("0,255,0,0.9"));
    ui->lb_sevlow2->setStyleSheet(cssL.arg("249,198,14,0.9"));
}

void MainWindow::on_updateLegends()
{
    createLegends();
}

void MainWindow::on_updateAngle()
{
    ui->picview1->on_updateAngle(ui->mesh1->yRot);
    ui->picview2->on_updateAngle(ui->mesh2->yRot);
}

void MainWindow::updateSeverityAndStyle(QLabel *l, float dLTRatio, float s)
{
    l->setText(QString::number(s, 'f', 3));
    l->setAlignment(Qt::AlignCenter);
    // FIXME
    if (s >= dLTRatio)
        l->setStyleSheet("color: black;"
                         "text-align: center;"
                         "background-color: rgba(255,0,0,0.9);"
                         "border-style: outset;"
                         "border-width: 1px;"
                         "border-radius: 10px;"
                         "padding: 3px;");
    else if (s >= 0.6 * dLTRatio)
        l->setStyleSheet("color: black;"
                         "text-align: center;"
                         "background-color: rgba(0,255,0,0.9);"
                         "border-style: outset;"
                         "border-width: 1px;"
                         "border-radius: 10px;"
                         "padding: 3px;");
    else
        l->setStyleSheet("color: black;"
                         "text-align: center;"
                         "background-color: rgba(249,198,14,0.9);"
                         "border-style: outset;"
                         "border-width: 1px;"
                         "border-radius: 10px;"
                         "padding: 3px;");
}

void MainWindow::on_pb_cut1_clicked()
{
    deleteBranches();
    updateSeverityAndStyle(ui->lb_ltratio1, dLTratio, getCurrentSeverity(deleted));
}

void MainWindow::on_pb_reset1_clicked()
{
    clearBranches();
    connectSignals();
}

void MainWindow::on_cb_analyze_stateChanged(int arg1)
{
    if (arg1)
    {
        analyze = 1;
    }
    else
    {
        analyze = 0;
    }
    emit updateFrame1();
}

void MainWindow::on_cb_analyze2_stateChanged(int arg1)
{
    if (arg1)
    {
        analyze2 = 1;
    }
    else
    {
        analyze2 = 0;
    }
    emit updateFrame2();
}

void MainWindow::on_pb_cut2_clicked()
{
    deleteBranches2();
    updateSeverityAndStyle(ui->lb_ltratio2, dLTratio, getCurrentSeverity(deleted2));
}

void MainWindow::on_pb_reset2_clicked()
{
    clearBranches2();
    connectSignals();
}

void MainWindow::on_sb_diameter_low_valueChanged(double arg1_cm)
{
    double arg1_m = cm2m(arg1_cm);
    // radius_l should be in m
    diameter_l = arg1_m;
    if (ui->sb_diameter_high->value() < arg1_cm)
    {
        // This triggers its value_changed call back
        ui->sb_diameter_high->setValue(arg1_cm+0.2);
    }


}

void MainWindow::on_sb_diameter_high_valueChanged(double arg1_cm)
{
    double arg1_m = cm2m(arg1_cm);
    // radius_m should be in m
    diameter_u = arg1_m;
    if (ui->sb_diameter_low->value() > arg1_cm)
    {
        // This triggers its value_changed call back
        ui->sb_diameter_low->setValue(arg1_cm-0.2);
    }

}

void MainWindow::on_pb_select_clicked()
{
    applyCriteria();
//    if (mar2 < 0) ui->lb_mar2->setText("--");
//    else ui->lb_mar2->setText(QString::number(mar2, 'f', 3));
    emit updateFrame2();
}

void MainWindow::on_cb_diameter_stateChanged(int arg1)
{
    if (arg1)
    {
        diameter_criteria = 1;
        ui->sb_diameter_high->setEnabled(true);
        ui->sb_diameter_low->setEnabled(true);
    }
    else
    {
        diameter_criteria = 0;
        ui->sb_diameter_high->setEnabled(false);
        ui->sb_diameter_low->setEnabled(false);
    }
}

void MainWindow::on_cb_angle_stateChanged(int arg1)
{
    if (arg1)
    {
        angle_criteria = 1;
        ui->sb_angle_high->setEnabled(true);
        ui->sb_angle_low->setEnabled(true);
    }
    else
    {
        angle_criteria = 0;
        ui->sb_angle_high->setEnabled(false);
        ui->sb_angle_low->setEnabled(false);
    }
}

void MainWindow::on_cb_ratio_stateChanged(int arg1)
{
    if (arg1)
    {
        ratio_criteria = 1;
        ui->sb_ratio_high->setEnabled(true);
        ui->sb_ratio_low->setEnabled(true);
    }
    else
    {
        ratio_criteria = 0;
        ui->sb_ratio_high->setEnabled(false);
        ui->sb_ratio_low->setEnabled(false);
    }
}

void MainWindow::on_cb_severity_stateChanged(int arg1)
{
    if (arg1)
    {
        severity_criteria = 1;
        ui->sb_severity->setEnabled(true);
        ui->co_approach->setEnabled(true);
    }
    else
    {
        severity_criteria = 0;
        ui->sb_severity->setEnabled(false);
        ui->co_approach->setEnabled(false);
    }
}

void MainWindow::on_sb_severity_valueChanged(double arg1)
{
    dLTratio2 = arg1;
    updateSeverityAndStyle(ui->lb_ltratio2, dLTratio, getCurrentSeverity(deleted2));
    createLegends();
}

void MainWindow::on_co_approach_currentIndexChanged(int index)
{
    switch(index)
    {
    case 0:
        severityMethod = LTR;
        break;
    case 1:
        severityMethod = MLD;
        break;
    }
}

void MainWindow::on_pb_cutCur1_clicked()
{
    deleteCurrentBranch(MANUAL);
    on_updateInfo1();
    updateSeverityAndStyle(ui->lb_ltratio1, dLTratio, getCurrentSeverity(deleted));
    emit updateFrame1();
}

void MainWindow::on_pb_cutCur2_clicked()
{
    deleteCurrentBranch(AUTO);
    updateSeverityAndStyle(ui->lb_ltratio2, dLTratio, getCurrentSeverity(deleted2));
    emit updateFrame2();
}

void MainWindow::on_cb_hlrank_clicked()
{
    ++highlight %= 2;
}

void MainWindow::on_cb_hlrank2_clicked()
{
    ++highlight2 %= 2;
}

void MainWindow::on_cb_and_or_clicked(bool checked)
{
    if (checked) and_or = 0;
    else and_or = 1;
}

void MainWindow::on_sb_angle_low_editingFinished()
{
    angle_l = ui->sb_angle_low->value();
    if (ui->sb_angle_high->value() < angle_l)
    {
        ui->sb_angle_high->setValue(angle_l+1);
        angle_u = angle_l+1;
    }
}

void MainWindow::on_sb_angle_high_editingFinished()
{
    angle_u = ui->sb_angle_high->value();
    if (ui->sb_angle_low->value() > angle_u)
    {
        ui->sb_angle_low->setValue(angle_u-1);
        angle_l = angle_u-1;
    }
}

void MainWindow::on_sb_ratio_low_editingFinished()
{
    ratio_l = ui->sb_ratio_low->value();
    if (ui->sb_ratio_high->value() < ratio_l)
    {
        ui->sb_ratio_high->setValue(ratio_l+0.01);
        ratio_u = ratio_l+0.01;
    }
}

void MainWindow::on_sb_ratio_high_editingFinished()
{
    ratio_u = ui->sb_ratio_high->value();
    if (ui->sb_ratio_low->value() > ratio_u)
    {
        ui->sb_ratio_low->setValue(ratio_u-0.01);
        ratio_l = ratio_u-0.01;
    }
}

void MainWindow::on_sb_angle_low_valueChanged(double arg1)
{
//    angle_l = arg1;
//    qDebug() << "1" << angle_l << angle_u;
//    if (ui->sb_angle_high->value() < arg1)
//    {
//        ui->sb_angle_high->setValue(arg1+1);
//    }
//    qDebug() << "2" << angle_l << angle_u;

}

void MainWindow::on_sb_angle_high_valueChanged(double arg1)
{
//    angle_u = arg1;
//    qDebug() << "3" << angle_l << angle_u;

//    if (ui->sb_angle_low->value() > arg1)
//    {
//        ui->sb_angle_low->setValue(arg1-1);
//    }
//    qDebug() << "4" << angle_l << angle_u;

}

void MainWindow::on_sb_ratio_low_valueChanged(double arg1)
{
//    ratio_l = arg1;
//    if (ui->sb_ratio_high->value() < arg1)
//    {
//        ui->sb_ratio_high->setValue(arg1+0.01);
//    }
}

void MainWindow::on_sb_ratio_high_valueChanged(double arg1)
{
//    ratio_u = arg1;
//    if (ui->sb_ratio_low->value() > arg1)
//    {
//        ui->sb_ratio_low->setValue(arg1-1);
//    }
}

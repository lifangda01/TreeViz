#include "mainwindow.h"
#include <QApplication>
#include "usda.h"

/* Mute qDebug */
//#define QT_NO_DEBUG_OUTPUT

/**********Tree Specific Parameters**********/
string treePath = "./resources/tree1/";
/********************************************/
QString picPath(QString::fromStdString(treePath)+"Pictures");
QString objName("tree.obj");

ModelLoader model;

int thisManyBranches;
Trunk trunk;
QVector<Branch> branches;

// Desired severity LT ratio
float dLTratio = DEF_SEVER; // For color coding in both auto & manual
float dLTratio2 = DEF_SEVER; // For sev criterial in auto
// Max allowed branch diameter in cm
float mar1 = -1.0;
float mar2 = -1.0;

// Severity pruning methods: LTR, MLD
int severityMethod = LTR;
// MLD approach parameters
float mld1 = MLD1, mld2 = MLD2, mld3 = MLD3;

// Ratios for qualifying outliers based on median and outlierMethod
// Deletion works the best
float xyzRatio = 0.2, rRatio = 0.2; // For branches
float tRatio = 1; // For trunk only
int outlierMethod = OR_DELETE;

// Radius and number of vertices in a circle
float cutRadius = 0.5f;
int cutPoints = 100;

//selected label
QVector<int> selected;
QVector<int> selected2;
QVector<int> deleted;
QVector<int> deleted2;

int analyze = 0;
int analyze2 = 0;
int highlight = 0;
int highlight2 = 0;

//current selected
int currIndex = -1;
float currAngle = -1.0;
float currRatio = -1.0;
float currRadius = -1.0;
int currIndex2 = -1;
float currAngle2 = -1.0;
float currRatio2 = -1.0;
float currRadius2 = -1.0;

// AND or OR label for applying criteria
// 0 for AND, 1 for OR
int and_or = 1;

//criteria selection
int angle_criteria = 0;
int ratio_criteria = 0;
int diameter_criteria = 0;
int size_criteria = 0;
int severity_criteria = 0;

// UI parameters
float angle_l = MIN_ANGLE;
float angle_u = MAX_ANGLE;
float ratio_l = MIN_RATIO;
float ratio_u = MAX_RATIO;
float diameter_l = MIN_DIAMETER;
float diameter_u = MIN_DIAMETER;
int branchTH = 0;
int main_window;
int main_window_2;
int treeModel_1 = 0;
int treeModel_2 = 0;

// Drawing color parameters
QVector<QVector3D> colorGradient;

// Branch ranking parameters
QVector<int> sev_rank;

int main(int argc, char *argv[])
{
    char *GL_version=(char *)glGetString(GL_VERSION);
    qDebug()<<GL_version;
    QApplication a(argc, argv);
    mainInit();
    MainWindow w;
    w.show();

    return a.exec();
}

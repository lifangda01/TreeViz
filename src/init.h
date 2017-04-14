#ifndef INIT_H
#define INIT_H

using namespace std;

#define PI 3.14159265
#define ERROR 0
#define OK 1

extern ModelLoader model;

extern int thisManyBranches;
extern string treePath;
extern QString picPath;
extern QString trunkTex;
extern QString branchTex;
extern QString objName;
extern QString projectPath;

extern Trunk trunk;
extern QVector<Branch> branches;

extern float dLTratio;
extern float dLTratio2;
extern float mar1, mar2;
extern int severityMethod;

extern float mld1, mld2, mld3;

extern float xyzRatio;
extern float rRatio;
extern float tRatio;
extern int outlierMethod;

extern QVector<int> selected;
extern QVector<int> selected2;
extern QVector<int> deleted;
extern QVector<int> deleted2;

extern int analyze;
extern int analyze2;
extern int highlight;
extern int highlight2;

extern int currIndex;
extern float currAngle;
extern float currRatio;
extern float currRadius;
extern int currIndex2;
extern float currAngle2;
extern float currRatio2;
extern float currRadius2;

extern int and_or;

extern int angle_criteria;
extern int ratio_criteria;
extern int diameter_criteria;
extern int size_criteria;
extern int severity_criteria;

extern float angle_l;
extern float angle_u;
extern float ratio_l;
extern float ratio_u;
extern float diameter_l;
extern float diameter_u;
extern int branchTH;

extern QVector<QVector3D> colorGradient;

extern float cutRadius;
extern int cutPoints;

extern QVector<int> sev_rank;

#endif

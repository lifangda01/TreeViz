#ifndef READINPUT_H
#define READINPUT_H

/* Get PLY point cloud data */
int getPLYData(Mesh *mesh, int branchNum);

/* Get all parameters for a branch cylinder */
int getCylinderData(Cylinder *cylinder, int branchNum);

/* Get circle data for trunk only */
int getTrunkCirclePara(TCircles *circles);

/* Get circle x,y,z,r for a given branch */
int getBranchCirclePara(BCircles *circles, int branchNum);

/* Get circle direction for a given branch */
int getBranchDirection(BCircles *circles, int branchNum);

/* Get branch to trunk angles */
int getBranchAngles();

/* Get branch to trunk diameter ratios */
int getBranchRatios();

/* Get cut point and normal for all branches */
int getBranchCuts();

/* Find outliers which are greater than ratio*median or smaller than median/ratio */
/* then returns a vector of outlier indices */
#define OR_DELETE 0     // Simply erase the outlier from vector
#define OR_REPLACE 1	// Replace outlier with median
#define OR_NEIGHBOR 2	// Replace with mean of two neighbors
QVector<int> medianOutlierFinder(QVector<float> &input, float ratio, float *median);

/* Given a vector of outlier indices in input, smooth the outlier */
/* Return new size of input */
int outlierRejector(QVector<float> &input, QVector<int> &outlierIndex, int method, float median);

/* Smoothing function for branch circles */
/* Uses finder and rejector */
/* Return new size of input */
int smoothBranchCircles(BCircles &circles, int method);
int smoothTrunkCircles(TCircles &circles, int method);

/* Generate color gradient of n colors from green to yellow to red */
void generateGradient(int n);

/* Main input reading for a tree */
void mainInit();

#endif

#ifndef SEVERITY_H
#define SEVERITY_H

/* Define which circle's diameter to pick for TCSA */
#define TCSA_HEIGHT 10

/* LT Ratio Approach / Max Limb Diameter Approach */
#define LTR 1
#define MLD 2

/* Constants in MD equation */
#define MLD1 -0.87
#define MLD2 0.553
#define MLD3 4.29

/* Return current severity, given deleted vector */
float getCurrentSeverity(QVector<int> d);

/* Select all the severe branches */
void cutSevereBranches(int method, float dLTratio);

/* Find branches to cut based on LT ratio */
QVector<int> findSevereBranches(Trunk &t, QVector<Branch> &b, int method, float dLTratio);

/* Return a sorted array of indices based on diameter */
QVector<int> findRadiusRank(QVector<Branch> &b);

/* Given all ranks and a branch index, find corresponding branch rank */
int getBranchRadiusRank(QVector<int> rank, int idx);

/* Return the largest visible branch index */
int findMaxVisibleBranch(QVector<Branch> &b, QVector<int> d);

#endif

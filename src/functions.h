#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void control_cb(int control);

void deleteBranches();
void deleteBranches2();

void deleteCurrentBranch(int mode);

/* Reset window */
void clearBranches();
void clearBranches2();

void applyCriteria();

void analyzeBranches();
void analyzeBranches2();

void updateCurrentInfo(int mode);

/* Find the number of files in a direction */
int findNumberOfFiles(QString path);

#endif

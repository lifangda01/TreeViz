#include "usda.h"

// Macro for checking whether x is within l-u
#define inRange(x,l,u) (x <= u && x >= l)

void deleteBranches(){
    for (int i = 0; i < thisManyBranches; i++){
        if (selected[i] == 1){
            deleted[i] = 1;
        }
    }
    currIndex = -1;
    currAngle = -1.0;
    currRatio = -1.0;
    currRadius = -1.0;
}

void deleteBranches2(){
    for (int i = 0; i < thisManyBranches; i++){
        if (selected2[i] == 1){
            deleted2[i] = 1;
        }
    }
}

void deleteCurrentBranch(int mode)
{
    switch (mode)
    {
    case MANUAL:
        if (currIndex >= 0)
        {
            deleted[currIndex] = 1;
            currIndex = -1;
            currAngle = -1.0;
            currRatio = -1.0;
            currRadius = -1.0;
        }
        break;
    case AUTO:
        if (currIndex2 >= 0)
        {
            deleted2[currIndex2] = 1;
            currIndex2 = -1;
            currAngle2 = -1.0;
            currRatio2 = -1.0;
            currRadius2 = -1.0;
        }
        break;
    }
}

void clearBranches(){
    for (int i = 0; i < thisManyBranches; i++){
        selected[i] = 0;
        deleted[i] = 0;
    }
    mar1 = -1.0;
    analyze = 0;
    currIndex = -1;
    currAngle = -1.0;
    currRatio = -1.0;
    currRadius = -1.0;
}

void clearBranches2(){
    for (int i = 0; i < thisManyBranches; i++){
        selected2[i] = 0;
        deleted2[i] = 0;
    }
    angle_l = MIN_ANGLE;
    angle_u = MAX_ANGLE;
    ratio_l = MIN_RATIO;
    ratio_u = MAX_RATIO;
    diameter_l = MIN_DIAMETER;
    diameter_u = MAX_DIAMETER;
    mar2 = -1.0;
    branchTH = 0;
    analyze2 = 0;
    currIndex2 = -1;
    currAngle2 = -1.0;
    currRatio2 = -1.0;
    currRadius2 = -1.0;
}

void applyCriteria(){

    // Automaticly pruning condition checking
    // Reset selected2 array and current branch info
    currIndex2 = -1;
    currAngle2 = -1.0;
    currRatio2 = -1.0;
    currRadius2 = -1.0;

    for (int i = 0; i < thisManyBranches; i++)
    {
        selected2[i] = 0;
    }

    int nCriteria = diameter_criteria + angle_criteria +
                    ratio_criteria + severity_criteria;

    // OR mode
    if (and_or == 1)
    {
        for (int i = 0; i < thisManyBranches; i++)
        {
            Branch *br = &branches[i];
            {
                qDebug() << br->BTratio << ratio_l << ratio_u;

                if (diameter_criteria && inRange(2*br->circles.r[0], diameter_l, diameter_u))
                {
                    selected2[i] = 1;
                }
                if (angle_criteria && inRange(br->angle, angle_l, angle_u))
                {
                    selected2[i] = 1;
                }
                if (ratio_criteria && inRange(br->BTratio, ratio_l, ratio_u))
                {
                    selected2[i] = 1;
                }
                if (severity_criteria)
                {
                    cutSevereBranches(severityMethod, dLTratio2);
                }
            }
        }
        return;
    }

    // AND mode
    if (and_or == 0)
    {
        for (int i = 0; i < thisManyBranches; i++)
        {
            Branch *br = &branches[i];
            {
                if (severity_criteria)
                {
                    cutSevereBranches(severityMethod, dLTratio2);
                }
                if (diameter_criteria && inRange(2*br->circles.r[0], diameter_l, diameter_u))
                {
                    selected2[i]++;
                }
                if (angle_criteria && inRange(br->angle, angle_l, angle_u))
                {
                    selected2[i]++;
                }
                if (ratio_criteria && inRange(br->BTratio, ratio_l, ratio_u))
                {
                    selected2[i]++;
                }
            }
        }

        for (int i = 0; i < thisManyBranches; i++)
        {
            selected2[i] = (selected2[i] == nCriteria) ? 1:0;
        }
        return;
    }
}

void analyzeBranches()
{
    analyze = 1;
}

void analyzeBranches2()
{
    analyze2 = 1;
}

void updateCurrentInfo(int mode)
{
    switch (mode)
    {
    case MANUAL:
        if (currIndex >= 0)
        {
            currAngle = branches[currIndex].angle;
            currRatio = branches[currIndex].BTratio;
            currRadius = branches[currIndex].circles.r[0];
        }
        else
        {
            currAngle = -1.0;
            currRatio = -1.0;
            currRadius = -1.0;
        }
        break;
    case AUTO:
        if (currIndex2 >= 0)
        {
            currAngle2 = branches[currIndex2].angle;
            currRatio2 = branches[currIndex2].BTratio;
            currRadius2 = branches[currIndex2].circles.r[0];
        }
        else
        {
            currAngle2 = -1.0;
            currRatio2 = -1.0;
            currRadius2 = -1.0;
        }
        break;
    }
}

int findNumberOfFiles(QString path)
{
    QDir brDir = QDir(path);
    QStringList brList = brDir.entryList(QStringList()<<"layer*", QDir::Dirs);
    return brList.size();
}

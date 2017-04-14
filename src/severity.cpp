#include "usda.h"

#define R2CSA(r) (3.1415926f*r*r)
#define R2CIR(r) (3.1415926f*2*r)

float getCurrentSeverity(QVector<int> d)
{
    float bSum = 0;
    float tcsa = R2CSA(trunk.diameter/2.0);
    for (int i = 0; i < thisManyBranches; i++)
        if (!d[i]) bSum += R2CSA(branches[i].circles.r[0]);
    return bSum / tcsa;
}

void cutSevereBranches(int method, float dLTratio)
{
    QVector<int> toCut;   // Indices of branches to be cut
    toCut = findSevereBranches(trunk, branches, method, dLTratio);
    for (int i = 0; i < toCut.size(); ++i)
    {
        selected2[toCut[i]] = 1;
    }
}

QVector<int> findSevereBranches(Trunk &t, QVector<Branch> &b, int method, float dLTratio)
{	
    QVector<int> toCut;

    // Find TCSA
    float tcsa, lcsa, tc, myLTratio;
    if (t.circleNum < TCSA_HEIGHT)
    {
        qCritical("Error: not enough trunk circles to determine TCSA!\n");
        return toCut;
    }
    tcsa = R2CSA(t.circles.r[TCSA_HEIGHT-1]);
    tc = R2CIR(t.circles.r[TCSA_HEIGHT-1]);
    qDebug() << "TCSA is " << tcsa << endl;

    // Initialize [LCSA]
    lcsa = 0;
    for (int i = 0; i < thisManyBranches; ++i)
    {
        lcsa += R2CSA(b[i].circles.r[0]);
    }

    // Get the rank of branches based on diameter
    QVector<int> rRank = findRadiusRank(b);

    switch (method)
    {
    case LTR:
        // Cut until desired LTratio is reached
        while((myLTratio = lcsa / tcsa) > dLTratio)
        {
            lcsa -= R2CSA(b[rRank.back()].circles.r[0]);
            qDebug() << "Cutting branch " << rRank.back() << ", LTratio now is " << myLTratio;
            toCut.push_back(rRank.back());
            rRank.pop_back();
        }
        // Record the remaining largest branch diameter
        //        mar2 = m2cm(b[rRank.back()].circles.r[0]);
        // Return the indices of branches to cut
        break;

    case MLD:
        // Calculate max remaining limb diameter
        float md = mld1 + mld2*tc + mld3*dLTratio;
        //        mar2 = md;
        qDebug() << "MD =" << md << "cm";
        for (int i = 0; i < thisManyBranches; ++i)
        {
            if (b[i].circles.r[0] * 2 > cm2m(md))
            {
                toCut.push_back(i);
            }
        }
        break;
    }
    return toCut;
}

QVector<int> findRadiusRank(QVector<Branch> &b)
{
    QVector<int> rRank (thisManyBranches, 0);

    for (int i = 0; i < rRank.size(); ++i)
    {
        rRank[i] = i;
    }

    int j, temp;
    for (int i = 1; i < rRank.size(); ++i)
    {
        j = i;
        temp = rRank[j];
        while(j > 0 && b[temp].circles.r[0] < b[rRank[j-1]].circles.r[0])
        {
            rRank[j] = rRank[j-1];
            j--;
        }
        rRank[j] = temp;
    }

    // Last element in rRank is the index of the branch with largest r
    //     for (int i = 0; i < rRank.size(); ++i)
    //        cout<<"Rank "<< i <<" is branch "<< rRank[i]<<endl;
    return rRank;
}

int getBranchRadiusRank(QVector<int> rank, int idx)
{
    for (int i = 0; i < rank.size(); ++i) {
        if (rank[i] == idx) return i;
    }
    return -1;
}

int findMaxVisibleBranch(QVector<Branch> &b, QVector<int> d)
{
    QVector<int> rRank = findRadiusRank(b);
    while(d[rRank.back()])
        rRank.pop_back();
    return rRank.back();
}

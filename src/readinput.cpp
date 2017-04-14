#include "usda.h"

int getCylinderData(Cylinder *cylinder, int branchNum)
{
    string line;
    stringstream filename;
    float t;
    filename.str("");
    filename << treePath << "Cylinder/feature_" << branchNum << ".txt";
    ifstream infile(filename.str().c_str());
    if(!infile)
    {
        cout << endl << "ERROR reading file: " << filename.str() << endl << endl;
        return ERROR;
    }

    getline(infile, line);
    istringstream att(line.c_str());
    att >> t;
    cylinder->at = t;

    getline(infile, line);
    istringstream btt(line.c_str());
    btt >> t;
    cylinder->bt = t;

    getline(infile, line);
    istringstream ctt(line.c_str());
    ctt >> t;
    cylinder->ct = t;

    getline(infile, line);
    istringstream rtt(line.c_str());
    rtt >> t;
    cylinder->rt = t;

    getline(infile, line);
    istringstream abt(line.c_str());
    abt >> t;
    cylinder->ab = t;

    getline(infile, line);
    istringstream bbt(line.c_str());
    bbt >> t;
    cylinder->bb = t;

    getline(infile, line);
    istringstream cbt(line.c_str());
    cbt >> t;
    cylinder->cb = t;

    getline(infile, line);
    istringstream rbt(line.c_str());
    rbt >> t;
    cylinder->rb = t;

    infile.close();
    return OK;
    // cout << "parameters: " << at.at(i) << " " << bt.at(i) << " " << ct.at(i) << " " << rt.at(i) << " " << ab.at(i) << " " << bb.at(i) << " " << cb.at(i) << " " << rb.at(i) << endl;
}

int getTrunkCirclePara(TCircles *circles)
{
    float test;
    stringstream filename;
    string line;
    int circleNum = 0;
    while(1)
    {
        filename.str("");
        filename << treePath << "TrunkCircles/paralayer_" << circleNum <<".txt";
        ifstream infile(filename.str().c_str());
//        if(!infile)
//        {
//            cout << endl << "ERROR reading file: " << filename.str() << endl << endl;
//            return ERROR;
//        }

        // Automatically break the loop when files are exhausted
        if(!getline(infile, line))
            break;
        istringstream x(line.c_str());
        x >> test;
        circles->x.push_back(test);

        if(!getline(infile, line))
            break;
        istringstream y(line.c_str());
        y >> test;
        circles->y.push_back(test);

        if(!getline(infile, line))
            break;
        istringstream r(line.c_str());
        r >> test;
        circles->r.push_back(test);

        circleNum++;
        // cout<<circles->x.back()<<", "<<circles->y.back()<<", "<<circles->r.back()<<endl;
    }
    // Return the number circles in trunk
    return circleNum;
}

int getBranchCirclePara(BCircles *circles, int branchNum)
{
    float test;
    stringstream filename;
    string line;
    int circleNum = 0;
    while(1)
    {
        filename.str("");
        filename << treePath << "BranchCircles/layers"<< branchNum << "/paralayer_" << circleNum <<".txt";
        ifstream infile(filename.str().c_str());
//        if(!infile)
//        {
//            cout << endl << "ERROR reading file: " << filename.str() << endl << endl;
//            return ERROR;
//        }

        if(!getline(infile, line))
            break;
        istringstream x(line.c_str());
        x >> test;
        circles->x.push_back(test);

        if(!getline(infile, line))
            break;
        istringstream y(line.c_str());
        y >> test;
        circles->y.push_back(test);

        if(!getline(infile, line))
            break;
        istringstream r(line.c_str());
        r >> test;
        circles->r.push_back(test);

        if(!getline(infile, line))
            break;
        istringstream z(line.c_str());
        z >> test;
        circles->z.push_back(test);

        circleNum++;
        // cout<<circles->x.back()<<", "<<circles->y.back()<<", "<<circles->z.back()<<", ";
        // cout<<circles->r.back()<<endl;
    }

    // Return the number of circles in this branch
    return circleNum;
}

int getBranchDirection(BCircles *circles, int branchNum)
{
    stringstream filename;
    string line;

    filename.str("");
    filename << treePath << "Cylinder/cylinderpara_"<< branchNum <<".txt";
    ifstream infile(filename.str().c_str());
    if(!infile)
    {
        cout << endl << "ERROR reading file: " << filename.str() << endl << endl;
        return ERROR;
    }

    getline(infile, line);
    getline(infile, line);
    getline(infile, line);

    getline(infile, line);
    istringstream a_t(line.c_str());
    a_t >> circles->a;

    getline(infile, line);
    istringstream b_t(line.c_str());
    b_t >> circles->b;

    getline(infile, line);
    istringstream c_t(line.c_str());
    c_t >> circles->c;

    return OK;
}

int getBranchCuts()
{
    for (int i = 0; i < thisManyBranches; i++)
    {
        Branch *br = &branches[i];
        br->cut = QVector3D(br->circles.x[0],br->circles.y[0],br->circles.z[0]);
        br->cutNorm = QVector3D(br->circles.a,br->circles.b,br->circles.c);
    }
    return OK;
}

QVector3D getClosestTrunkUpVector(float z)
{
    static int i = 0;
    int iThis = 0;
    // FIXME
    float delta = 3.2f / trunk.circleNum;
    float hl = (i)*delta-1.2;
    float hh = (i+1)*delta-1.2;
    int win = 2;

    // Find the closest trunk circle
    while (!( z >= hl && z < hh ))
    {
        i++;
        iThis++;
        if (i >= trunk.circleNum-win)
            i = 0;
        // Break in case of infinite loop
        if (iThis > trunk.circleNum)
            return QVector3D(0.0, 0.0, 1.0f);
        hl = (i)*delta-1.2;
        hh = (i+1)*delta-1.2;
    }

    // They are really close to vertical
    // FIXME
    QVector3D up = QVector3D(trunk.circles.x[i+win] - trunk.circles.x[i],
            trunk.circles.y[i+win] - trunk.circles.y[i],
            0.1f);
//    return up.normalized();
    // FIXME
    return QVector3D(0, 0, 1.0f);
}

int getBranchAngles()
{
    QVector3D up;
    for (int i = 0; i < thisManyBranches; ++i)
    {
        Branch *br = &branches[i];
        QVector3D norm = QVector3D(br->circles.a,br->circles.b,br->circles.c);
        up = getClosestTrunkUpVector(br->circles.z[0]);
        //        qDebug() << up << br->circles.z[0];
        float theta = qAcos(QVector3D::dotProduct(up, norm) / norm.length());
        br->angle = qRadiansToDegrees(theta);
    }
    return OK;
}

int getBranchRatios()
{
    for (int i = 0; i < thisManyBranches; ++i)
    {
        Branch *br = &branches[i];
        br->BTratio = br->circles.r[0] / trunk.diameter;
    }
    return OK;
}

QVector<int> medianOutlierFinder(QVector<float> &input, float ratio, float *median)
{
    // Find the median
    QVector<float> input_copy = input;
    size_t size = input_copy.size();
    QVector<int> outlierIndex;

    if (size < 3) // If too few data points are available, just return
        return outlierIndex;

    sort(input_copy.begin(), input_copy.end());

    if (size % 2 == 0)
        *median = (input_copy[size / 2 - 1] + input_copy[size / 2]) / 2;
    else
        *median = input_copy[size / 2];

    // cout<<"median is "<<*median<<endl;

    for (int i = 0; i < size; ++i)
    {
        if (abs((input[i] - *median) / (*median)) > ratio)
        {
            outlierIndex.push_back(i);
        }
    }
    return outlierIndex;
}

int outlierRejector(QVector<float> &input, QVector<int> &outlierIndex, int method, float median = -1)
{
    if (outlierIndex.empty() || input.empty()) return input.size();
    switch (method)
    {
    case OR_REPLACE:
        if (median == 0) perror("Error: Please provide a valid median.");
        for (int i = 0; i < outlierIndex.size(); ++i)
        {
            input[outlierIndex[i]] = median;
        }
        return input.size();

    case OR_NEIGHBOR: // becomes problematic when neighbor is also bad but this wont be often if input is not that bad
        if (input.size() < 3)
            return input.size();
        if (outlierIndex[0] == 0)
        {
            input[0] = input[1];
        }
        if (outlierIndex.back() == input.size()-1)
        {
            input[input.size()-1] = input[input.size()-2];
        }
        for (int i = 0; i < outlierIndex.size(); ++i)
        {
            if (outlierIndex[i] != 0 && outlierIndex[i] != (input.size()-1))
                input[outlierIndex[i]] = (input[outlierIndex[i]-1] + input[outlierIndex[i]+1]) / 2;
        }
        return input.size();

    case OR_DELETE:
        for (int i = 0; i < outlierIndex.size(); ++i)
        {
            input.erase(input.begin() + outlierIndex[i] - i);
        }
        return input.size();
    }
    return input.size();
}

int smoothBranchCircles(BCircles &circles, int method)
{
    QVector<int> xOutlierIndices,yOutlierIndices,zOutlierIndices,rOutlierIndices;

    xOutlierIndices = medianOutlierFinder(circles.x, xyzRatio, &circles.xMedian);
    yOutlierIndices = medianOutlierFinder(circles.y, xyzRatio, &circles.yMedian);
    zOutlierIndices = medianOutlierFinder(circles.z, xyzRatio, &circles.zMedian);
    rOutlierIndices = medianOutlierFinder(circles.r, rRatio, &circles.rMedian);

    switch(method)
    {
    case OR_DELETE:
    {
        QVector<int> outlierIndices;
        outlierIndices.reserve(xOutlierIndices.size() + yOutlierIndices.size()
                               + zOutlierIndices.size() + rOutlierIndices.size());

        outlierIndices += xOutlierIndices;
        outlierIndices += yOutlierIndices;
        outlierIndices += zOutlierIndices;
        outlierIndices += rOutlierIndices;

        sort(outlierIndices.begin(), outlierIndices.end());
        outlierIndices.erase(unique(outlierIndices.begin(), outlierIndices.end()), outlierIndices.end());

        outlierRejector(circles.x, outlierIndices, method, circles.xMedian);
        outlierRejector(circles.y, outlierIndices, method, circles.yMedian);
        outlierRejector(circles.z, outlierIndices, method, circles.zMedian);
        outlierRejector(circles.r, outlierIndices, method, circles.rMedian);

        return circles.x.size();
    }
    default:
        /*
      cout<<"num of outliers after merging: "<<outlierIndices.size()<<endl;

      for (int i = 0; i < outlierIndices.size(); ++i)
      {
        cout<<outlierIndices[i]<<" ";
      }
      cout<<endl;
      */
        outlierRejector(circles.x, xOutlierIndices, method, circles.xMedian);
        outlierRejector(circles.y, yOutlierIndices, method, circles.yMedian);
        outlierRejector(circles.z, zOutlierIndices, method, circles.zMedian);
        outlierRejector(circles.r, rOutlierIndices, method, circles.rMedian);
        /*
      cout<<"num of circle after smoothing: " << circles.x.size()<<endl;
      cout<<"circle parameters after smoothing: "<<endl;
      for (int i = 0; i < circles.x.size(); ++i)
      {
        cout<<circles.x[i]<<", "<<circles.y[i]<<", "<<circles.z[i]<<", ";
        cout<<circles.r[i]<<endl;
      }
      */
    }
    return circles.x.size();
}

int smoothTrunkCircles(TCircles &circles, int method)
{
    QVector<int> outlierIndices;
    outlierIndices = medianOutlierFinder(circles.r, tRatio, &circles.rMedian);

    outlierRejector(circles.x, outlierIndices, method, circles.xMedian);
    outlierRejector(circles.y, outlierIndices, method, circles.yMedian);
    outlierRejector(circles.r, outlierIndices, method, circles.rMedian);

    return circles.x.size();
}

void generateGradient(int n)
{
    colorGradient.clear();
    // RGB
    QVector3D color = QVector3D(0.0f, 1.0f, 0.0f);
    float step = 2 * 1.0f / (float)n;
    while(true)
    {
        if (color[0] < 1.0f)
        {
            colorGradient.push_back(color);
            color[0] += step;
            color[0] = (color[0] > 1.0f) ? 1.0f : color[0];
            continue;
        }
        else if (color[1] > 0.0f)
        {
            colorGradient.push_back(color);
            color[1] -= step;
            color[1] = (color[1] < 0.0f) ? 0.0f : color[1];
            continue;
        }
        else
            break;
    }
    qDebug() << "Number of colors in gradient:" << colorGradient.size();
}

void mainInit()
{
    trunk = Trunk();
    branches.clear();

    QString qTreePath;
    qTreePath = QString::fromStdString(treePath);
    qDebug() << "Loading " << qTreePath + objName;
    // Reinitiate the model so that old model gets erased
    ModelLoader *tempModel;
    tempModel = new ModelLoader;
    model = *tempModel;
    if (!model.Load(qTreePath + objName, ModelLoader::AbsolutePath))
    {
        qCritical() << "Could not load model";
        QMessageBox::critical((QWidget *)NULL, QObject::tr("No Tree Found"),
                              QObject::tr("Cannot find tree model files."),
                              QMessageBox::Ok);
        return;
    }

    thisManyBranches = findNumberOfFiles(qTreePath + "BranchCircles/");
    qDebug()<<"Number of branches: "<<thisManyBranches;
    branches.resize(thisManyBranches);

    // Initiate label vectors
    selected.fill(0, thisManyBranches);
    selected2.fill(0, thisManyBranches);
    deleted.fill(0, thisManyBranches);
    deleted2.fill(0, thisManyBranches);

    // Get features for cylinders: x,y,z,a,b,c,r,h
    //    for (int i = 0; i < thisManyBranches; ++i)
    //    {
    //        getCylinderData(&branches[i].cylinder, i);
    //    }

    // Get circle data
    if (getTrunkCirclePara(&trunk.circles) == ERROR)
    {
        qCritical() << "Could not load trunk ";
        QMessageBox::critical((QWidget *)NULL,QObject::tr("No Trunk Found"),
                              QObject::tr("Cannot find trunk model."),
                              QMessageBox::Ok);
        return;
    }
    trunk.diameter = 2*trunk.circles.r[TCSA_HEIGHT-1];
    trunk.circleNum = smoothTrunkCircles(trunk.circles, outlierMethod);
    for (int i = 0; i < thisManyBranches; ++i)
    {
        if (getBranchCirclePara(&branches[i].circles, i) == ERROR ||
            getBranchDirection(&branches[i].circles, i) == ERROR)
        {
            qCritical() << "Could not load branch" << i;
            QMessageBox::critical((QWidget *)NULL, QObject::tr("No Branch Found"),
                                  QObject::tr("Cannot find branch model."),
                                  QMessageBox::Ok);
            return;
        }

        // Reject obvious outliers and updata number of circles
//        branches[i].circleNum = smoothBranchCircles(branches[i].circles, outlierMethod);
        qDebug() << "Branch " << i << "diameter: " << 2*branches[i].circles.r[0];
    }
    qDebug() << "Trunk diameter: " << trunk.diameter;

    //    // Sort branch diameter in ascending order
    //    for (int i = 0; i < thisManyBranches; i++){
    //        rb_copy.push_back(branches[i].cylinder.rb);
    //    }
    //    std::sort(rb_copy.begin(), rb_copy.end());

    //    // Get the rank of branch diameter
    //    for(int i = 0; i < thisManyBranches; i++){
    //        for(int j = 0; j < thisManyBranches; j++){
    //            if (std::fabs(branches[i].cylinder.rb - rb_copy.at(j)) < std::numeric_limits<double>::epsilon()){
    //                rb_rank.push_back(j);
    //            }
    //        }
    //    }

    //    for(int i = 0; i < thisManyBranches; i++){
    //        qDebug() << "Branch " << i << "diameter ranks: " << rb_rank.at(i);
    //    }

    sev_rank = findRadiusRank(branches);

    // Get the angles of branches, depend on cylinder
    getBranchAngles();

    // Get the limb to trunk diameter ratio
    getBranchRatios();

    // Get the cutting points of branches
    getBranchCuts();

    // Get the gradient of colors for highlighting branches
    generateGradient(thisManyBranches);
}

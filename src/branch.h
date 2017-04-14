#ifndef BRANCH_H
#define BRANCH_H

#define MIN_ANGLE 0
#define MAX_ANGLE 180
#define MIN_RATIO 0
#define MAX_RATIO 1.5
#define MIN_DIAMETER 0
#define MAX_DIAMETER 0.1
#define MIN_SEVER 0
#define MAX_SEVER 10
#define DEF_SEVER 1.25
#define MIN_SEVER 0
#define MAX_SEVER 10

#define cm2m(x) (x/100)
#define m2cm(x) (x*100)

#define d2r(x)  (x/2.0f)
#define r2d(x)  (x*2.0f)

#define AUTO 1
#define MANUAL 2

class Cylinder {
public:
	// features for cylinders: x,y,z,a,b,c,r,h
	float at, bt, ct, rt, ab, bb, cb, rb;
	Cylinder(){
		at = bt = ct = rt = ab = bb = cb = rb = 0;
	}
};

class Mesh {
public:
	// point cloud mesh parameters
    QVector<float> x,y,z;
	// indices of connnected 
    QVector<float> face1,face2,face3;
	Mesh(){
		x.clear(); y.clear(); z.clear();
		face1.clear(); face2.clear(); face3.clear();
	}
};

class BCircles {
public:
    // x,y,z coordinates, r, and a,b,c normals
    QVector<float> x,y,z,r;
	float a,b,c;
	float xMedian,yMedian,zMedian,rMedian;
	BCircles(){
		x.clear(); y.clear(); z.clear(); r.clear();
		a = b = c = 0;
		xMedian = yMedian = zMedian = rMedian = 0;
	}
};

class TCircles {
public:
	// x,y coordinates and r 
    QVector<float> x,y,r;
	float xMedian,yMedian,rMedian;
	TCircles(){
		x.clear(); y.clear(); r.clear();
		xMedian = yMedian = rMedian = 0;
	}
};

class Branch {
public:
	Cylinder cylinder;
	BCircles circles;
	int circleNum;
	float angle;
	float BTratio;
    QVector3D cut;
    QVector3D cutNorm;
	Branch(){
		cylinder = Cylinder();
		circles = BCircles();
		circleNum = angle = BTratio = 0;
        cut = QVector3D();
        cutNorm = QVector3D();
	}
};

class Trunk {
public:
	TCircles circles;
	int circleNum;
    float diameter;
	Trunk(){
		circles = TCircles();
		circleNum = 0;
	}
};

#endif

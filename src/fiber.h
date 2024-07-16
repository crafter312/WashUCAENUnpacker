#ifndef fiber_
#define fiber_

#include <iostream>
#include <math.h>
#include "CAENd5202.h"

using namespace std;

class fiber {
public:

	fiber();
	int ix;
	int iy;
	float sumhorz;
	float sumvert;

	float x,y,r,theta,phi;
	float thetadeg,phideg;

	// Index of hit (type dataTiming) in vector with max ToT value
	int posmaxhorz;
	int posmaxvert;

	bool make_2d(Event*, Event*, float);
};

#endif

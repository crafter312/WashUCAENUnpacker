#ifndef fiber_
#define fiber_

#include <iostream>
#include <math.h>
#include "CAENd5202.h"

using namespace std;

class fiber {
public:

	fiber(float);
	void reset();
	bool has_data;
	int ix;
	int iy;
	int multhorz;
	int multvert;
	float sumhorz;
	float sumvert;

	float x,y,r,theta,phi;
	float thetadeg,phideg;
	float distance;

	// Max value positions in dataTiming which is type vector<eventTiming>
	int posmaxhorz;
	int posmaxvert;

	bool make_2d(Event*, Event*);
};

#endif

#ifndef fiber_
#define fiber_

using namespace std;

class Event;

class fiber {
public:

	fiber();
	int ix;
	int iy;
	double sumhorz;
	double sumvert;

	double x, y, r;
	double thetadeg, phideg;

	// Index of hit (type dataTiming) in vector with max ToT value
	int posmaxhorz;
	int posmaxvert;

	bool make_2d(Event*, Event*, double);
};

#endif

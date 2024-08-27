#ifndef fiber_
#define fiber_

#include <vector>

using namespace std;

class Event;

class fiber {
public:

	fiber();
	int ix;
	int iy;
	int multTrimmedX;
	int multTrimmedY;
	double xdev;
	double ydev;
	double avgfibx;
	double avgfiby;
	double sumhorz;
	double sumvert;
	double tstampdiff;
	vector<double> tdiffx;
	vector<double> tdiffy;
	bool badtx;
	bool badty;
	

	double x, y, r;
	double thetadeg, phideg;

	// Index of hit (type dataTiming) in vector with max ToT value
	int posmaxhorz;
	int posmaxvert;

	// Indices for hits that pass all the various time gates
	vector<int> xindices;
	vector<int> yindices;

	bool make_2d(Event*, Event*, double);

private:

	void clear();
};

#endif

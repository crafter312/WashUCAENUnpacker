#ifndef fiber_
#define fiber_

#include <tuple>
#include <vector>

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
	std::vector<double> tdiffx;
	std::vector<double> tdiffy;
	bool badtx;
	bool badty;
	

	double x, y, r;
	double thetadeg, phideg;

	// Index of hit (type dataTiming) in vector with max ToT value
	int posmaxhorz;
	int posmaxvert;

	// Indices for hits that pass all the various time gates
	std::vector<int> xindices;
	std::vector<int> yindices;

	std::tuple<bool, bool> make_2d(Event&, Event&, double); // returns { horz, vert } to match constructor order
	bool isGoodSingleRed(Event&);
	bool isGoodSingleBlue(Event&);

private:

	void clear();
	bool isGoodSingle(Event&, int, int/*, double, double*/);
};

#endif

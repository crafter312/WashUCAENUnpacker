#ifndef det_
#define det_
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "histo.h"
#include "CAENd5202.h"
#include "fiber.h"

using namespace std;

class det {

public:
	det(float);
	~det();

	bool unpack(ifstream *);
	void MatchEvents();
	
	histo* Histo;
	Event* SIPMevent;
	fiber* Fiber;

	//make vector of last 50 events saved
	vector<Event*> redbuffevents;
	vector<Event*> bluebuffevents;

	int Nsingles = 0;
	int Nmatched = 0;

	long nevts;

private:
	float distance; // distance in mm between target and detector

	// Scale values for gain matching
	double bluegains[64];
	double redgains[64];

	void ReadGains(string, double*);
};
#endif

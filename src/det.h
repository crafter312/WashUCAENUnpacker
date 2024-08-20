#ifndef det_
#define det_

#include <fstream>
#include <string>
#include <vector>

class Event;
class fiber;
class histo;

using namespace std;

class det {

public:
	det(histo*, double);
	~det();
	histo* Histo;

	bool unpack(ifstream*);
	void MatchEvents();
	
	Event* SIPMevent;
	fiber* Fiber;

	// Vector of most recent 20 events
	vector<Event*> redbuffevents;
	vector<Event*> bluebuffevents;

	int Nsingles = 0;
	int Nmatched = 0;
	int Nskipped = 0;

	long nevts;

private:
	// Scale values for gain matching
	double bluegains[64];
	double redgains[64];

	float distance;

	void ReadGains(string, double*);
};

#endif

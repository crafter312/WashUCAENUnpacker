#ifndef det_
#define det_

#include "CAENd5202.h"
#include "fiber.h"
#include "histo.h"

#include <fstream>
#include <string>
#include <vector>

class det {

public:
	det(histo*, double);
	~det();
	histo* Histo;

	bool unpack(std::ifstream*);
	void MatchEvents();
	
	Event SIPMevent;
	fiber Fiber;

	// Vector of most recent 20 events
	std::vector<Event> redbuffevents;
	std::vector<Event> bluebuffevents;

	int nUncorrBlue = 0;
	int nUncorrRed  = 0;
	int nMatched    = 0;

	int goodBlue    = 0;
	int goodRed     = 0;
	int goodMatched = 0;

	long nevts;

private:
	// Scale values for gain matching
	double bluegains[64];
	double redgains[64];

	float distance;

	void ReadGains(std::string, double*);
};

#endif

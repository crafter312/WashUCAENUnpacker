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

class det
{
 public:
  det(histo* Histo);
  ~det();
  histo* Histo;

  bool unpack(ifstream *);
	void MatchEvents();
  
  Event* SIPMevent;
	fiber* Fiber;

	//make vector of last 50 events saved
	vector<Event*> redbuffevents;
	vector<Event*> bluebuffevents;

	int Nsingles = 0;
	int Nmatched = 0;

  long nevts;
  
};
#endif

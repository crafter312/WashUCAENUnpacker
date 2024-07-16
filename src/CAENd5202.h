#ifndef _eventCAEN
#define _eventCAEN

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <time.h>
#include <chrono>
#include <stdexcept>
#include <iomanip>

using namespace std;

// Data structure for holding event data in timing mode. May need different structure for spectroscopy mode.
struct eventTiming {
  unsigned char chan;
	unsigned int pos{0};
  int ToA{-1}; // Time of arrival, could be a float if calibrated
  short ToT{-1}; // Time over threshold, could be a float if calibrated
	double ToTmatched{-1};

  // use these getter functions to get chan and type as shorts
  short getChan() { return (short)chan; }

  void clear() {
    pos = 0;
    ToA = -1;
    ToT = -1;
		ToTmatched = -1;
  }
};

class Event {

public:
  Event();
	Event(Event* rhs);

	// For debugging
	string Print(bool b) const;
  
  // This template method does the unpacking work. It will take in a position p in
  // the buffer and the variable t it expects with type T, saving the data in the
  // buffer to the indicated variable t, then advancing the buffer.
  // reads in data Little Endian
  template<class T> void set_val(T& t, char*& p)
  {
    t = *reinterpret_cast<T*>(p);
    p += sizeof(T);
  }
  
	void clear();

	long ReadHeader(ifstream*);
  long ReadEventFromStream(ifstream*, double*, double*); // input file, redgains, bluegains

  void set_short(unsigned short &, char*&);
  void set_24bit(unsigned int &, char*&);

	unsigned short GetNHits() { return NHits; }
  eventTiming GetTimingEvent(unsigned int i) { return dataTiming[i]; };
	unsigned char GetBoardID() { return boardID; }
	double GetTimeStamp() { return timeStamp; }
  
private:
	// Event info (Timing Mode)
  unsigned char boardID;
  double timeStamp; // start of Tref window in ms

	// Event Data
	unsigned short NHits;           // number of hits
  vector<eventTiming> dataTiming; // vector of hits
};

#endif

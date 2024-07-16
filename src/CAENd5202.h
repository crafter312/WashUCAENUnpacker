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
  unsigned char type; // 0x10, if only the ToA value is saved for that channel; 0x20, if only the ToT value is saved for that channel; 0x30, if both ToA and ToT values are saved
  int ToA{-1}; // Time of arrival, could be a float if calibrated
  short ToT{-1}; // Time over threshold, could be a float if calibrated
	float ToTmatched{-1};

  // use these getter functions to get chan and type as shorts
  short getChan() { return (short)chan; }
  short getType() { return (short)type; }

  void clear() {
    pos = 0;
    ToA = -1;
    ToT = -1;
		ToTmatched = -1;
  }
};

class Event {

public:
  Event(string, string);
	Event(Event* rhs);

	// For debugging
	string Print(bool b) const;
  
  // This template class does the unpacking work. It will take in a position p in
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
  long ReadEventFromStream(ifstream*);

  void set_short(unsigned short &, char*&);
  void set_24bit(unsigned int &, char*&);

  eventTiming GetTimingEvent(unsigned int);
	unsigned char GetBoardID() { return boardID; }
	double GetTimeStamp() { return timeStamp; }

	// Event Data
  vector<eventTiming> dataTiming; // vector of hits
	unsigned short NHits;           // number of hits
  
private:
  bool firstline=true;

	// Scale values for gain matching
	float bluegains[64];
	float redgains[64];

	// Event info (Timing Mode)
  unsigned char boardID;
  double timeStamp; // start of Tref window in ms

	void ReadGains(string, float*);
};

#endif

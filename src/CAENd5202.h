#ifndef _eventCAEN
#define _eventCAEN

#include <fstream>
#include <functional>
#include <string>
#include <vector>

using namespace std;

// Data structure for holding event data in timing mode. May need different structure for spectroscopy mode.
class eventTiming {

public:

	eventTiming();

	// get chan as short
  short getChan() { return (short)chan; }

  unsigned char chan;    // channel # from DT5202
	unsigned int pos{0};   // channel # converted to fiber #
  int ToA{-1};           // time of arrival, could be a float if calibrated
  short ToT{-1};         // time over threshold, could be a float if calibrated
	double ToTmatched{-1}; // scaled ToT calculated from gain matching parameters

  void clear();

};

class Event {

public:
  Event();
	Event(const Event& rhs);

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
  void set_short(unsigned short &, char*&);
  void set_24bit(unsigned int &, char*&);

	long ReadHeader(ifstream*);
  long ReadEventFromStream(ifstream*, double*, double*); // input file, redgains, bluegains

	eventTiming FindMax(function<bool(eventTiming, eventTiming)>);	

	unsigned short GetNHits() { return NHits; }
  eventTiming& GetTimingEvent(unsigned int i) { return dataTiming[i]; };
	unsigned char GetBoardID() { return boardID; }
	double GetTimeStamp() { return timeStamp; }
  
private:
	// Event info (Timing Mode)
  unsigned char boardID;
  double timeStamp; // start of Tref window in us

	// Event Data
	unsigned short NHits;           // number of hits
  vector<eventTiming> dataTiming; // vector of hits

};

#endif

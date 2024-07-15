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

// Data structure for holding event data in timing mode. May need different structure for spectroscopy mode.
struct eventSpecTiming {
  unsigned char chan;
  int pos{-1};
  unsigned char type; // 0x10, if only the ToA value is saved for that channel; 0x20, if only the ToT value is saved for that channel; 0x30, if both ToA and ToT values are saved
  unsigned short low{0};
  unsigned short high{0};
  float ToA{-1}; // Time of arrival, could be a float if calibrated
  float ToT{-1}; // Time over threshold, could be a float if calibrated

  // use these getter functions to get chan and type as shorts
  short getChan() { return (short)chan; }
  short getType() { return (short)type; }

  void clear() {
    low = 0;
    high = 0;
    pos = -1;
    ToA = -1;
    ToT = -1;
  }
};



class Event {
public:
  Event(string, string);
	Event(Event* rhs);
  //all GetX and print functions useful for debugging
  //unsigned short  GetBoard() const;

  string Print(bool b) const;
  
  //This template class does the unpacking work. It will take in a position p in
  //the buffer and the variable t it expects with type T, saving the data in the
  //buffer to the indicated variable t, then advancing the buffer.
  //reads in data Little Endian
  template<class T> void set_val(T& t, char*& p)
  {
    t = *reinterpret_cast<T*>(p);
    p += sizeof(T);
  }
  
  long ReadEventFromStream(ifstream*);
  void clear();

  long ReadHeader(ifstream*);
  long ReadDataTimingMode(ifstream*);
  long ReadDataSpecTimingMode(ifstream*);

  void set_short(unsigned short &, char*&);
  void set_24bit(unsigned int &, char*&);

	unsigned char GetAcqMode() { return acqMode; }

	double GetTimeStamp() { return timeStamp; }
	unsigned char GetBoardID() { return boardID; }

  unsigned short NHits; // Number of recorded hits
  eventTiming GetTimingEvent(unsigned int);
  eventSpecTiming GetSpecTimingEvent(unsigned int);

	// Event Data
  vector<eventTiming> dataTiming;
  vector<eventSpecTiming> dataSpecTiming;
  
private:
  bool firstline=true;

	// Scale values for gain matching
	float bluegains[64];
	float redgains[64];

  // File Header
  unsigned short formatVersion;
  unsigned int softwareVersion; // When read gets software version and acq mode
  unsigned short modelnumber;
  unsigned short runnum;
  unsigned char acqMode; // 0x01 for SpectroscopyMode; 0x02 for TimingMode; 0x03 for Spectroscopy+TimingMode; 0x04 for CountingMode
  unsigned char timeUnit;
  unsigned short NChannels; // NChannels is the total number of channels of the Energy histogram
  float timeConversion; // Time conversion is the conversion value between LSB and ns for the timing information. For A5202/DT5202 this value is 1 LSB = 0.5 ns
  time_t startAcq; // The ”Start Acquisition” information is expressed in ms with reference to the UnixEpoch time

  // Event info (Timing Mode)
  unsigned char boardID;
  double timeStamp;
  unsigned short eventSize;
  unsigned long TrigID;
  unsigned long chanMask;

	void ReadGains(string, float*);
};

#endif

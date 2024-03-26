//File created to unpack the CAEN D5202 into a class that stores all
//of the data for each event.
//created by Charlie Fallon 12/1/2023

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <time.h>
#include <chrono>
#include "CAENd5202.h"

//Definitions for the Event class, holds one event.
//define a bunch of GetX methods of the Event class
//unsigned short  Event::GetBoard()       const { return fBoard;       }



string Event::Print(bool b = false) const
{
  ostringstream oss;
  oss << "board:"<< boardID << "\t timeStamp:" << timeStamp << "\t NHits:" << NHits;

  if (acqMode == 0x03)
    oss << "\tTrigID:" << hex << TrigID << dec;
  oss << endl;

  // column headers
  if (b) {
    oss << "EV# | channel | data type | ToA | ToT";
    if (acqMode == 0x03)
      oss << " | low | high";
    oss << endl;
  }

  // hit loop
  eventTiming ev;
  eventSpecTiming evSpec;
  for (int i = 0; i < NHits; i++) {
    oss << i;
    if (acqMode == 0x03) {
      evSpec = dataSpecTiming[i];
      oss << " " << evSpec.getChan() << " " << evSpec.getType() << " " << evSpec.ToA << " " << evSpec.ToT << " " << evSpec.low << " " << evSpec.high << endl;
      continue;
    }
    ev = dataTiming[i];
    oss << " " << ev.getChan() << " " << ev.getType() << " " << ev.ToA << " " << ev.ToT << endl;
  }

  return oss.str();
}




Event::Event()
{
  clear();
}

//reads one event from the stream and saves it to the private variables
long Event::ReadEventFromStream(ifstream *pfs)
{
  pfs->peek();
  if(!pfs->good())
    return -1;

  // Get initial position
  std::streampos initialPos = pfs->tellg();
  if (firstline)
  {
    ReadHeader(pfs);
    firstline = false;
  }

  if (acqMode == 0x02)
    ReadDataTimingMode(pfs);
  else if (acqMode == 0x03)
    ReadDataSpecTimingMode(pfs);
  

  // Get final position
  std::streampos finalPos = pfs->tellg();

  // Return # of bytes read
  return long(finalPos - initialPos);
}


//set_vals in need Big Endian style
void Event::set_short(unsigned short &t, char*& p)
{
  t = (*p++ << 8);
  t = t | *p++;
}
void Event::set_24bit(unsigned int &t, char*& p)
{
  t = (*p++ << 8);
  t = (t | *p++) << 8;
  t = t | *p++;
}

eventTiming Event::GetTimingEvent(unsigned int i) {
  return dataTiming[i];
}
eventSpecTiming Event::GetSpecTimingEvent(unsigned int i) {
  return dataSpecTiming[i];
}

long Event::ReadHeader(ifstream *pfs)
{
  size_t evtsize = 25;
  char buf[evtsize];
  pfs->read((char*)buf, evtsize);
  char* pbuf = buf;

  set_short(formatVersion, pbuf);
  cout << "Format Version: " << hex << formatVersion << dec << endl; 
  set_24bit(softwareVersion, pbuf);
  cout << "Software Version: " << hex << softwareVersion << dec << endl;
  
  set_val(modelnumber, pbuf);
  if (modelnumber != 5202)
  {
    cout << "you have the wrong file type" << endl;
    abort();
  }

  set_val(runnum, pbuf);
  cout << "Reading Run#" << runnum << endl;

  set_val(acqMode, pbuf);
  cout << "Acquisition Mode: " << (short)acqMode << endl;

  set_val(NChannels, pbuf);
  cout << "Number of channels: " << NChannels << endl;

  //if (timeUnit) all times in units of ns, else times are in channels
  set_val(timeUnit, pbuf);
  cout << "Time Unit: " << (short)timeUnit << endl;

  set_val(timeConversion, pbuf);
  cout << "Time conversion: " << timeConversion << "ns (should be 0.5ns)" << endl;

  set_val(startAcq, pbuf);

  startAcq /= 1000;
  printf("data taken on %s", ctime(&startAcq));
  //printf("%s", asctime(gmtime(&startAcq)));
}


long Event::ReadDataTimingMode(ifstream *pfs)
{
  //peak at the first part to deterime how large of a buffer to create
  size_t peaksize = 2;
  char peaker[peaksize];
  pfs->read((char*)peaker, peaksize);
  char* pbuf = peaker;
  set_val(eventSize, pbuf);

  //create the buffer (size 2 less because we already read the first part)
  char buf[eventSize-2];
  pfs->read((char*)buf, eventSize-2);
  pbuf = buf;

  set_val(boardID, pbuf);
  set_val(timeStamp, pbuf);
  set_val(NHits, pbuf);

  eventTiming Ev;
  for (int n=0; n<NHits; n++)
  {
    Ev.clear();
    
    set_val(Ev.chan, pbuf);
    set_val(Ev.type, pbuf);
    set_val(Ev.ToA, pbuf);
    set_val(Ev.ToT, pbuf);
    dataTiming.push_back(Ev);
  }

}

long Event::ReadDataSpecTimingMode(ifstream *pfs)
{
  //peak at the first part to deterime how large of a buffer to create
  size_t peaksize = 2;
  char peaker[peaksize];
  pfs->read((char*)peaker, peaksize);
  char* pbuf = peaker;
  set_val(eventSize, pbuf);

  //create the buffer (size 2 less because we already read the first part)
  char buf[eventSize-2];
  pfs->read((char*)buf, eventSize-2);
  pbuf = buf;

  set_val(boardID, pbuf);
  set_val(timeStamp, pbuf);
  set_val(TrigID, pbuf);
  set_val(chanMask, pbuf);

  NHits = 0;
  int bytesleft = eventSize-27;
  eventSpecTiming Ev;
  while (bytesleft > 0)
  {
    Ev.clear();
    set_val(Ev.chan, pbuf);
    set_val(Ev.type, pbuf);
    bytesleft -= 2;

    //first byte of data type says if you have low/high/both data
    if ((Ev.type & 0x03) == 0x01)
    {
      set_val(Ev.low, pbuf);
      bytesleft -= 2;
    }
    else if ((Ev.type & 0x03) == 0x02)
    {
      set_val(Ev.high, pbuf);
      bytesleft -= 2;
    }
    else if ((Ev.type & 0x03) == 0x03)
    {
      set_val(Ev.low, pbuf);
      set_val(Ev.high, pbuf);
      bytesleft -= 4;
    }
    //second byte of data type says if you have ToA/ToT/both data
    if (((Ev.type>>4) & 0x03) == 0x01)
    {
      set_val(Ev.ToA, pbuf);
      bytesleft -= 4;
    }
    else if (((Ev.type>>4) & 0x03) == 0x02)
    {
      set_val(Ev.ToT, pbuf);
      bytesleft -= 4;
    }
    else if (((Ev.type>>4) & 0x03) == 0x03)
    {
      set_val(Ev.ToA, pbuf);
      set_val(Ev.ToT, pbuf);
      bytesleft -= 8;
    }

    dataSpecTiming.push_back(Ev);
    NHits++;
  }
}


void Event::clear()
{
  // Event Header (Timing Mode)
  eventSize = 0;
  boardID = 0;
  timeStamp = 0;
  NHits = 0; // Number of recorded hits
  dataTiming.clear();
  dataSpecTiming.clear();
}


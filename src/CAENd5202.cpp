// Modified on 15 July by Henry Webb

#include "CAENd5202.h"

// File created to unpack the CAEN DT5202 into a class that stores all
// of the data for each event. This version of the unpacker is designed
// to work only with timing-only events (acqMode==0x02)

string Event::Print(bool b = false) const
{
  ostringstream oss;
  oss << "board:" << (int)boardID << "\t timeStamp:" << fixed << timeStamp << "\t NHits:" << NHits << endl;

  // column headers
  if (b)
    oss << "EV# | channel | pos | data type | ToA | ToT" << endl;

  // hit loop
  eventTiming ev;
  for (int i = 0; i < NHits; i++) {
    oss << i;
    ev = dataTiming[i];
    oss << " " << ev.getChan() << " " << ev.pos << " " << ev.getType() << " " << ev.ToA << " " << ev.ToT << endl;
  }

  return oss.str();
}

Event::Event(string bname, string rname)
{
  clear();

	ReadGains(bname, bluegains);
	ReadGains(rname, redgains);
}

Event::Event(Event* rhs)
{
	// Copy event header info
	boardID = rhs->boardID;
	timeStamp = rhs->timeStamp;
	NHits = rhs->NHits;	

	// Copy event data
	dataTiming = rhs->dataTiming;

	copy(rhs->bluegains, rhs->bluegains+64, bluegains);
	copy(rhs->redgains, rhs->redgains+64, redgains);
}

void Event::ReadGains(string ifname, float* arr) {
	ifstream ifile;
	ifile.open(ifname, ios::in);
	if (!ifile.is_open())
		throw invalid_argument("Supplied input file does not open properly");
	
	float data;
	for (int i = 0; i < 64; i++) {
		ifile >> data;
		if (ifile.eof())
			throw invalid_argument("Supplied input file shorter than expected length");
		arr[i] = data;
	}
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

// Reads and checks header variables
// THIS MUST BE EXECUTED ONCE BEFORE READING AN EVENT!!
long Event::ReadHeader(ifstream *pfs)
{
  size_t evtsize = 25;
  char buf[evtsize];
  pfs->read((char*)buf, evtsize);
  char* pbuf = buf;

	// Copy default formatting
	ios init(NULL);
	init.copyfmt(cout);

	// Declare local header variables
	unsigned short formatVersion;
  unsigned int softwareVersion;
	unsigned short modelnumber; // should be 5202 to match DT5202 board
	unsigned short runnum;
	unsigned char acqMode;      // 0x01 for SpectroscopyMode; 0x02 for TimingMode; 0x03 for Spectroscopy+TimingMode; 0x04 for CountingMode
	unsigned short NChannels;   // NChannels is the total number of channels of the Energy histogram
	unsigned char timeUnit;     // specifies time unit of ToT and ToA (see Janus manual, sec. 3.8.1)
  float timeConversion;       // conversion value between LSB and ns for the timing information (1 LSB = 0.5 ns for A5202/DT5202)
  time_t startAcq;            // time of acquisition start in ms since Unix Epoch

  set_short(formatVersion, pbuf);
  cout << "Format version: " << hex << formatVersion << dec << endl; 
  set_24bit(softwareVersion, pbuf);
  cout << "Software version: " << hex << softwareVersion << dec << endl;
  
  set_val(modelnumber, pbuf);
	cout << "Model number: " << modelnumber << endl;
  if (modelnumber != 5202)
		throw invalid_argument("Invalid model number (should be 5202)");

  set_val(runnum, pbuf);
  cout << "Reading run #" << runnum << endl;

  set_val(acqMode, pbuf);
  cout << "Acquisition Mode: " << setw(2) << setfill('0') << hex << (short)acqMode << endl;
	if (acqMode != 0x02) {
		char tempbuf[4];
		sprintf(tempbuf, "%02x", acqMode);
		throw invalid_argument("Invalid acquisition mode (should be 0x02): " + string(tempbuf));
	}

	// Reset formatting
	cout.copyfmt(init);

  set_val(NChannels, pbuf);
  cout << "Number of channels: " << NChannels << endl;

  //If timeUnit==1 all times in units of ns, else times are in channels (LSB)
  set_val(timeUnit, pbuf);
  cout << "Time Unit: " << (short)timeUnit << endl;

  set_val(timeConversion, pbuf);
  cout << "Time conversion: " << timeConversion << " ns (should be 0.5 ns)" << endl;

  set_val(startAcq, pbuf);
  startAcq /= 1000;
  printf("data taken on %s", ctime(&startAcq));
}

// Reads one event from the stream and saves it to the private variables
long Event::ReadEventFromStream(ifstream *pfs)
{
	pfs->peek();
  if (!pfs->good())
    return -1;

	// Get initial position
	std::streampos initialPos = pfs->tellg();

  // Peak at the first part to deterime how large of a buffer to create
  size_t peaksize = 2;
  char peaker[peaksize];
  pfs->read((char*)peaker, peaksize);
  char* pbuf = peaker;
	unsigned short eventSize;
  set_val(eventSize, pbuf);

  // Create the buffer (size 2 less because we already read the first part)
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
		Ev.pos = (((unsigned int)Ev.chan - ((unsigned int)Ev.chan % 2)) / 2) + (((unsigned int)Ev.chan % 2) * 32);
    set_val(Ev.type, pbuf);
    set_val(Ev.ToA, pbuf);
    set_val(Ev.ToT, pbuf);
		Ev.ToTmatched = ((float)Ev.ToT) * (((boardID == 0) * redgains[Ev.pos]) + ((boardID == 1) * bluegains[Ev.pos]));
    dataTiming.push_back(Ev);
  }

	// Get final position
  std::streampos finalPos = pfs->tellg();

	// Return # of bytes read
  return long(finalPos - initialPos);
}

void Event::clear()
{
  // Event Header (Timing Mode)
  boardID = 0;
  timeStamp = 0;
  NHits = 0; // Number of recorded hits
  dataTiming.clear();
}




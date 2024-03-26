// in the det class, the data file is stored into vectors of events.
// the event class is detailed in eventCAEN.cpp
// created 12/1/2023 by Charlie Fallon

#include "det.h"

// constructor
det::det(histo * Histo1)
{
  Histo = Histo1;
  SIPMevent = new Event();
}

det::~det()
{
  // empty for now, in the future will need to delete classes
}

// the unpack class handles the opened data file, unpacks each event
bool det::unpack(ifstream *pevtfile)
{ 
  nevts = 0;
  long nbytes = 0;
  nbytes = SIPMevent->ReadEventFromStream(pevtfile);

	// declare common variables
	double timeStamp;
	float tot;
	float toa;

	// handle spec-timing mode
	unsigned char acqMode = SIPMevent->GetAcqMode();
	if(acqMode == 0x03) {
		Histo->InitSpecMode();

		eventSpecTiming evSpec;
		unsigned short low;
		unsigned short high;
		
		// loop through file
		for(;;) {
			if(nbytes == -1) break;
			cout << "event # " << nevts << endl;

			timeStamp = SIPMevent->GetTimeStamp();
    	evSpec = SIPMevent->GetSpecTimingEvent(0);
    	low = evSpec.low;
			high = evSpec.high;
    	tot = evSpec.ToT;
    	toa = evSpec.ToA;
    	if(low > 0) Histo->lg_hist->Fill(low);
    	if(tot > -1) Histo->tot_hist->Fill(tot);
    	if(toa > -1) Histo->toa_hist->Fill(toa);
    	if(low > 0 && tot > -1) Histo->tot_lg_hist->Fill(low, tot);
    	Histo->FillTree(timeStamp, low, high, tot, toa);

    	SIPMevent->clear();
    	nevts++;

  		nbytes = SIPMevent->ReadEventFromStream(pevtfile);
		}

		return true;
	}

	// else handle timing-only mode
	eventTiming evSpec;
  for(;;)
  {
		if(nbytes == -1) break;
		cout << "event # " << nevts << endl;
    //cout << SIPMevent->Print(true) << endl;

		timeStamp = SIPMevent->GetTimeStamp();
    evSpec = SIPMevent->GetTimingEvent(0);
    tot = evSpec.ToT;
    toa = evSpec.ToA;
    if(tot > -1) Histo->tot_hist->Fill(tot);
    if(toa > -1) Histo->toa_hist->Fill(toa);
    Histo->FillTree(timeStamp, tot, toa);

    //if (nevts == 1)
    //  abort();

    SIPMevent->clear();
    nevts++;

  	nbytes = SIPMevent->ReadEventFromStream(pevtfile);
  }

  return true;
}

// in the det class, the data file is stored into vectors of events.
// the event class is detailed in eventCAEN.cpp
// created 12/1/2023 by Charlie Fallon
// modified 7/15/2024 by Henry Webb

#include "det.h"

#define CONFIGPATH "/home/Li6Webb/Desktop/SFA/caenUnpacker/config/"

// Constructor
det::det(histo * Histo1) {
  Histo = Histo1;
  SIPMevent = new Event();
	Fiber = new fiber(100.); // <-dist in mm from the target

	ReadGains(string(CONFIGPATH) + "blue_gain_matching.txt", bluegains);
	ReadGains(string(CONFIGPATH) + "red_gain_matching.txt", redgains);
}

// Destructor
det::~det() {}

// Read scaling values for gain matching from file
void det::ReadGains(string ifname, double* arr) {
	ifstream ifile;
	ifile.open(ifname, ios::in);
	if (!ifile.is_open())
		throw invalid_argument("Supplied input file does not open properly");
	
	double data;
	for (int i = 0; i < 64; i++) {
		ifile >> data;
		if (ifile.eof())
			throw invalid_argument("Supplied input file shorter than expected length");
		arr[i] = data;
	}
}

// Unpack class handles the opened data file, unpacks each event
bool det::unpack(ifstream *pevtfile) { 
  nevts = 0;
  long nbytes = 0;

	// Read file header
	// THIS MUST BE DONE ONCE BEFORE READING AN EVENT!
  nbytes = SIPMevent->ReadHeader(pevtfile);

	// Declare common variables
	double timeStamp;
	float tot;
	float toa;
	unsigned char chan;

	// Event loop (timing-only mode)
	eventTiming ev;
  for(;;) {
		nbytes = SIPMevent->ReadEventFromStream(pevtfile, redgains, bluegains); // reads next event
		if(nbytes == -1) break; // stop at end of file

  	Event* SIPMeventcur = new Event(SIPMevent);
		//cout << SIPMeventcur->Print(true) << endl;

		timeStamp = SIPMeventcur->GetTimeStamp();
    ev = SIPMeventcur->GetTimingEvent(0);
    tot = ev.ToTmatched;
    toa = ev.ToA;
		chan = ev.chan;

    if(tot > -1) Histo->tot_hist->Fill(tot);
    if(toa > -1) Histo->toa_hist->Fill(toa);
    Histo->FillTree(timeStamp, SIPMeventcur->GetBoardID(), SIPMeventcur->dataTiming);

		// Add event to buffer
		if (SIPMeventcur->GetBoardID() == 0) {
			redbuffevents.insert(redbuffevents.begin(), SIPMeventcur);

			// limit red vector size to 20
			if (redbuffevents.size() > 20)
				redbuffevents.pop_back(); //TODO potentially a huge bug, I don't know if I have actually deleted the variable and freed up memory
		}
		else if (SIPMeventcur->GetBoardID() == 1) {
			bluebuffevents.insert(bluebuffevents.begin(), SIPMeventcur);

			// limit blue vector size to 20
			if (bluebuffevents.size() > 20)
				bluebuffevents.pop_back();
		}

		MatchEvents();

    SIPMevent->clear();
    Histo->clear();
    nevts++;
  }

  return true;
}

void det::MatchEvents() {

	bool matched = false;
	double tstampdiff = 0;
	
	for (int i = 0; i<redbuffevents.size(); i++) {
		for (int j = 0; j<bluebuffevents.size(); j++) {

			tstampdiff = abs(redbuffevents[i]->GetTimeStamp() - bluebuffevents[j]->GetTimeStamp());

			if (tstampdiff < 2) {
				// (Event* horizontal, Event* vertical) <-this is how horz and vertical are assigned
				Fiber->make_2d(bluebuffevents[j], redbuffevents[i]);

				//	Write histograms and tree here
			  Histo->Fiber_ixiy->Fill(Fiber->ix, Fiber->iy);
			  Histo->Fiber_xy->Fill(Fiber->x, Fiber->y);
        Histo->Fiber_toax->Fill(bluebuffevents[j]->dataTiming[Fiber->posmaxhorz].ToA);
        Histo->Fiber_toay->Fill(redbuffevents[i]->dataTiming[Fiber->posmaxvert].ToA);
				Histo->FillMatchedTree(Fiber, redbuffevents[i], bluebuffevents[j]);

				// Plot hit map for individual events
        if (Nmatched == 0) {
          int PHraw, ToAraw, pos;
          for (int k=0; k<bluebuffevents[j]->NHits; k++) {
		        PHraw = bluebuffevents[j]->dataTiming[k].ToTmatched;
            ToAraw = bluebuffevents[j]->dataTiming[k].ToA;
            pos = bluebuffevents[j]->dataTiming[k].pos;
            Histo->Fiber_postotx->AddBinContent(Histo->Fiber_postotx->GetBin(pos), PHraw);
            Histo->Fiber_postoax->AddBinContent(Histo->Fiber_postoax->GetBin(pos), ToAraw);
          }
          for (int k=0; k<redbuffevents[i]->NHits; k++) {
		        PHraw = redbuffevents[i]->dataTiming[k].ToTmatched;
            ToAraw = redbuffevents[i]->dataTiming[k].ToA;
            pos = redbuffevents[i]->dataTiming[k].pos;
            Histo->Fiber_postoty->AddBinContent(Histo->Fiber_postoty->GetBin(pos), PHraw);
            Histo->Fiber_postoay->AddBinContent(Histo->Fiber_postoay->GetBin(pos), ToAraw);
          }
        }

				// Increment matched and unmatched counts
				Nmatched += 1;
				Nsingles += redbuffevents.size() - i + bluebuffevents.size() - j - 2;

				// Delete all events older than matched pair
		    redbuffevents.erase(redbuffevents.begin()+i, redbuffevents.end());
		    bluebuffevents.erase(bluebuffevents.begin()+j, bluebuffevents.end());

				return;
			}
		}
	}
}










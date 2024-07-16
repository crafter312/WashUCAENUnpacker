// in the det class, the data file is stored into vectors of events.
// the event class is detailed in eventCAEN.cpp
// created 12/1/2023 by Charlie Fallon
// modified 7/15/2024 by Henry Webb

#include "det.h"

#define CONFIGPATH "/home/Li6Webb/Desktop/SFA/caenUnpacker/config/"

// Constructor
det::det(float d) {
	distance = d;

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
	eventTiming hit;
	float tot;
	float toa;
	unsigned char chan;

	// Event loop (timing-only mode)
	for (;;) {
		nbytes = SIPMevent->ReadEventFromStream(pevtfile, redgains, bluegains); // reads next event
		if (nbytes == -1) break; // stop at end of file

		Event* SIPMeventcur = new Event(SIPMevent);
		//cout << SIPMeventcur->Print(true) << endl;

		// Loop through hits in event
		int nhits = (int)SIPMeventcur->GetNHits();
		for (int i = 0; i < nhits; i++) {
			hit = SIPMeventcur->GetTimingEvent(i);
			tot = hit.ToTmatched;
			toa = hit.ToA;
			chan = hit.chan;

			// Fill histograms
			if (toa > -1) Histo->toa_hist->Fill(toa);
		}
		
		// Fill output tree
		Histo->FillTree(*SIPMeventcur);

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

	// Advance declaration of variables
	double tstampdiff = 0;
	Event* tempev;
	eventTiming ev;
	int PHraw, ToAraw, pos;
	
	// Loop through event buffers
	for (int i = 0; i<redbuffevents.size(); i++) {
		for (int j = 0; j<bluebuffevents.size(); j++) {

			tstampdiff = abs(redbuffevents[i]->GetTimeStamp() - bluebuffevents[j]->GetTimeStamp());

			if (tstampdiff < 2) {
				// (Event* horizontal, Event* vertical) <-this is how horz and vertical are assigned
				Fiber->make_2d(bluebuffevents[j], redbuffevents[i], distance);

				//	Write histograms and tree here
				Histo->Fiber_ixiy->Fill(Fiber->ix, Fiber->iy);
				Histo->Fiber_xy->Fill(Fiber->x, Fiber->y);
				Histo->Fiber_toax->Fill(bluebuffevents[j]->GetTimingEvent(Fiber->posmaxhorz).ToA);
				Histo->Fiber_toay->Fill(redbuffevents[i]->GetTimingEvent(Fiber->posmaxvert).ToA);
				Histo->FillMatchedTree(Fiber, redbuffevents[i], bluebuffevents[j]);

				// Increment matched and unmatched counts
				Nmatched += 1;
				Nsingles += redbuffevents.size() - i + bluebuffevents.size() - j - 2;

				// Delete all events older than matched pair
				redbuffevents.erase(redbuffevents.begin()+i, redbuffevents.end());
				bluebuffevents.erase(bluebuffevents.begin()+j, bluebuffevents.end());
				
				if (Nmatched > 1) return;

				// Plot hit map for individual events
				tempev = Histo->GetBlueEvent();
				for (int k = 0; k<tempev->GetNHits(); k++) {
					ev = tempev->GetTimingEvent(k);
					Histo->Fiber_postotx->AddBinContent(Histo->Fiber_postotx->GetBin(ev.pos), ev.ToT);
					Histo->Fiber_postoax->AddBinContent(Histo->Fiber_postoax->GetBin(ev.pos), ev.ToA);
				}
				tempev = Histo->GetRedEvent();
				for (int k = 0; k<tempev->GetNHits(); k++) {
					ev = tempev->GetTimingEvent(k);
					Histo->Fiber_postoty->AddBinContent(Histo->Fiber_postoty->GetBin(ev.pos), ev.ToT);
					Histo->Fiber_postoay->AddBinContent(Histo->Fiber_postoay->GetBin(ev.pos), ev.ToA);
				}

				return;
			}
		}
	}
}










// in the det class, the data file is stored into vectors of events.
// the event class is detailed in eventCAEN.cpp
// created 12/1/2023 by Charlie Fallon
// modified 7/15/2024 by Henry Webb

#include "det.h"

#include "CAENd5202.h"
#include "fiber.h"
#include "histo.h"

#include <iostream>

#define CONFIGPATH "/home/Li6Webb/Desktop/SFA/caenUnpacker/config/"

// Constructor
det::det(histo * Histo1, double d) {
	Histo = Histo1;
	SIPMevent = new Event();
	Fiber = new fiber();
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
	unsigned int pos;

	// Event loop (timing-only mode)
	bool val;
	for (;;) {
		nbytes = SIPMevent->ReadEventFromStream(pevtfile, redgains, bluegains); // reads next event
		if (nbytes == -1) break; // stop at end of file

		Event* SIPMeventcur = new Event(SIPMevent);
		//cout << SIPMeventcur->Print(true) << endl;

		// Loop through hits in event
		unsigned char boardID = SIPMeventcur->GetBoardID();
		int nhits = (int)SIPMeventcur->GetNHits();
		for (int i = 0; i < nhits; i++) {
			hit = SIPMeventcur->GetTimingEvent(i);
			tot = hit.ToTmatched;
			toa = hit.ToA;
			pos = hit.pos;

			// Fill histograms
			if (toa > -1)
				Histo->toa_hist->Fill(toa);
			if (tot > -1 && boardID == 0)
				Histo->tot_summary_red->Fill(pos, tot);
			else if (tot > -1 && boardID == 1)
				Histo->tot_summary_blue->Fill(pos, tot);
		}
		
		// Fill output tree
		Histo->FillTree(*SIPMeventcur);

		// Add event to buffer
		if (boardID == 0) {
			redbuffevents.insert(redbuffevents.begin(), SIPMeventcur);

			// limit red vector size to 20
			if (redbuffevents.size() > 20)
				redbuffevents.pop_back(); //TODO potentially a huge bug, I don't know if I have actually deleted the variable and freed up memory
		}
		else if (boardID == 1) {
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
				bool val = Fiber->make_2d(bluebuffevents[j], redbuffevents[i], distance);

				//// PROGRAM CRASHES IF YOU REMOVE THE FOLLOWING IF STATEMENT ////
				// it is currently completely useless, as the "fiber::make_2d"
				// function above never at any point returns false. Note that the
				// program worked just fine before I added this, and now that I've
				// added it I can't remove it for some completely nonsensical reason.
				if (!val) {
					cout << "FALSE RETURN" << endl;
					redbuffevents.erase(redbuffevents.begin()+i, redbuffevents.end());    // the problem occurs if
					bluebuffevents.erase(bluebuffevents.begin()+j, bluebuffevents.end()); // you remove these lines
					Nskipped++;
					return; // (was meant to) return early if not a valid matched event (doesn't pass time gates, for example)
				}

				//	Write histograms and tree here
				Histo->FillMatchedTree(Fiber, redbuffevents[i], bluebuffevents[j]);
				Histo->Fiber_ixiy->Fill(Fiber->ix, Fiber->iy);
				Histo->Fiber_xy->Fill(Fiber->x, Fiber->y);

				ev = bluebuffevents[j]->GetTimingEvent(Fiber->posmaxhorz);
				Histo->Fiber_tot_summary_x->Fill(ev.pos, ev.ToTmatched);
				Histo->Fiber_toax->Fill(ev.ToA);

				ev = redbuffevents[i]->GetTimingEvent(Fiber->posmaxvert);
				Histo->Fiber_tot_summary_y->Fill(ev.pos, ev.ToTmatched);
				Histo->Fiber_toay->Fill(ev.ToA);

				// Increment matched and unmatched counts
				Nmatched += 1;
				Nsingles += redbuffevents.size() - i + bluebuffevents.size() - j - 2;

				// Delete all events older than matched pair
				redbuffevents.erase(redbuffevents.begin()+i, redbuffevents.end());
				bluebuffevents.erase(bluebuffevents.begin()+j, bluebuffevents.end());
				
				if (Nmatched > 1) return;

				// Plot hit map for individual events
				tempev = Histo->GetBlueEvent();
				double temppos;
				for (int k = 0; k<tempev->GetNHits(); k++) {
					ev = tempev->GetTimingEvent(k);
					temppos = -1*(ev.pos-0.5)*0.5 + 16; //mm
					Histo->Fiber_totx->AddBinContent(Histo->Fiber_totx->GetXaxis()->FindBin(temppos), ev.ToTmatched);
					Histo->Fiber_postotx->AddBinContent(Histo->Fiber_postotx->GetXaxis()->FindBin(ev.pos), ev.ToTmatched);
					Histo->Fiber_postoax->AddBinContent(Histo->Fiber_postoax->GetXaxis()->FindBin(ev.pos), ev.ToA);
				}
				tempev = Histo->GetRedEvent();
				for (int k = 0; k<tempev->GetNHits(); k++) {
					ev = tempev->GetTimingEvent(k);
					temppos = -1*(ev.pos-0.5)*0.5 + 16; //mm
					Histo->Fiber_toty->AddBinContent(Histo->Fiber_toty->GetXaxis()->FindBin(temppos), ev.ToTmatched);
					Histo->Fiber_postoty->AddBinContent(Histo->Fiber_postoty->GetXaxis()->FindBin(ev.pos), ev.ToTmatched);
					Histo->Fiber_postoay->AddBinContent(Histo->Fiber_postoay->GetXaxis()->FindBin(ev.pos), ev.ToA);
				}

				return;
			}
		}
	}
}










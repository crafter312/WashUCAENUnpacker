// in the det class, the data file is stored into vectors of events.
// the event class is detailed in eventCAEN.cpp
// created 12/1/2023 by Charlie Fallon

#include "det.h"

#define CONFIGPATH "/home/Li6Webb/Desktop/SFA/caenUnpacker/config/"

// constructor
det::det(histo * Histo1)
{
  Histo = Histo1;
  SIPMevent = new Event(string(CONFIGPATH) + "blue_gain_matching.txt", string(CONFIGPATH) + "red_gain_matching.txt");
	Fiber = new fiber(100.); // <-dist in mm from the target
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
	unsigned char chan;

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
			chan = evSpec.chan;
			
    	if(low > 0) Histo->lg_hist->Fill(low);
    	if(tot > -1) Histo->tot_hist->Fill(tot);
    	if(toa > -1) Histo->toa_hist->Fill(toa);
    	if(low > 0 && tot > -1) Histo->tot_lg_hist->Fill(low, tot);
    	Histo->FillTree(timeStamp, SIPMevent->GetBoardID(), SIPMevent->dataSpecTiming);

    	SIPMevent->clear();
      Histo->clear();
    	nevts++;

  		nbytes = SIPMevent->ReadEventFromStream(pevtfile);
		}

		return true;
	}

	// else handle timing-only mode
	eventTiming evSpec;
  for(;;)
  {
  	Event* SIPMeventcur = new Event(SIPMevent);

		if(nbytes == -1) break;
		//if(nevts > 10) return true;

		//cout << endl;
		//cout << endl;
		//cout << "event # " << nevts << endl;
    //cout << SIPMevent->Print(true) << endl;
		//cout << "NHits " << SIPMeventcur->NHits << endl;






		timeStamp = SIPMeventcur->GetTimeStamp();
    evSpec = SIPMeventcur->GetTimingEvent(0);
    tot = evSpec.ToTmatched;
    toa = evSpec.ToA;
		chan = evSpec.chan;

		
		//cout << SIPMeventcur->Print(true) << endl;

    if(tot > -1) Histo->tot_hist->Fill(tot);
    if(toa > -1) Histo->toa_hist->Fill(toa);
    Histo->FillTree(timeStamp, SIPMevent->GetBoardID(), SIPMevent->dataTiming);


		//inside SIPMevent it has      std::vector<eventTiming> dataTiming;
		//load in eventTiming into buffer of events
		if (SIPMeventcur->GetBoardID() == 0)
		{
			//cout << "load red" << endl;
			redbuffevents.insert(redbuffevents.begin(), SIPMeventcur);
			if (redbuffevents.size() > 20){
				redbuffevents.pop_back(); //throw away last element so array size is always 50
			}
			
		}
		if (SIPMeventcur->GetBoardID() == 1)
		{
			//cout << "load blue" << endl;
			bluebuffevents.insert(bluebuffevents.begin(), SIPMeventcur);
			if (bluebuffevents.size() > 20){
				bluebuffevents.pop_back(); //throw away last element so array size is always 50
				//TODO potentially a huge bug, I don't know if I have actually deleted the variable and freed up memory
			}
		}

		MatchEvents();

    SIPMevent->clear();
    Histo->clear();
    nevts++;

  	nbytes = SIPMevent->ReadEventFromStream(pevtfile);
  }

  return true;
}


void det::MatchEvents(){

	bool matched = false;
	double tstampdiff = 0; //TODO check timestamps are doubles not unsigned longs
	//cout << "redbuffevents.size(): " << redbuffevents.size() << "   bluebuffevents.size(): " << bluebuffevents.size() << endl;
	for (int i=0; i<redbuffevents.size(); i++){
		for (int j=0; j<bluebuffevents.size(); j++){

			tstampdiff = abs(redbuffevents[i]->GetTimeStamp() - bluebuffevents[j]->GetTimeStamp());
			//cout << "redbuffevents[i]->GetTimeStamp() " << fixed << redbuffevents[i]->GetTimeStamp() << endl;
			//cout << "bluebuffevents[i]->GetTimeStamp() " << fixed <<  bluebuffevents[j]->GetTimeStamp() << endl;
			//cout << "checking events i,j = " << i << "," << j << " in buff with tstampdiff = " << tstampdiff << endl;

			//cout << "redbuffevents[j]->print()" << redbuffevents[i]->Print(true) << endl;
			//cout << "bluebuffevents[j]->print()" << bluebuffevents[j]->Print(true) << endl;

			//TODO don't forget about single layer events, we want to look at these for efficiency
			//TODO delete events, maybe just delete all events once we find a match
			//keep track of timestamp for matches, throw away all events older than that, but count those up (these are singles)

			if (tstampdiff < 2){ //timestamp is in microsec?
				//cout << "matched with tstampdiff " << tstampdiff << endl;
										//(Event* horizontal, Event* vertical) <-this is how horz and vertical are assigned
				Fiber->make_2d(bluebuffevents[j], redbuffevents[i]); 

				
				

				//write histograms here
			  Histo->Fiber_ixiy->Fill(Fiber->ix, Fiber->iy);
			  Histo->Fiber_xy->Fill(Fiber->x, Fiber->y);
        Histo->Fiber_toax->Fill(bluebuffevents[j]->dataTiming[Fiber->posmaxhorz].ToA);
        Histo->Fiber_toay->Fill(redbuffevents[i]->dataTiming[Fiber->posmaxvert].ToA);
				Histo->FillMatchedTree(Fiber, redbuffevents[i], bluebuffevents[j]);

				//TODO plot with timestamp difference and ToA difference

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

        matched = true;
				Nmatched += 1;
			}
			if (matched) {
        Nsingles += redbuffevents.size() - i + bluebuffevents.size() - j - 2;
		    redbuffevents.erase(redbuffevents.begin()+i, redbuffevents.end());
		    bluebuffevents.erase(bluebuffevents.begin()+j, bluebuffevents.end());
        break;
      }
		}
		if (matched) break;
	}

	//once a match is found, count up the unmatched events and then throw away all oler events because these are singles
	if (matched){
		
	}
}










#include "histo.h"

// TODO:
// summary spectrum (like Janus)
// 1D for each channel
// multiplicity (number of hits in an event)
// event profile? (weighted average, needed for better hit localization)
// same channel multiplicity
// ToA difference between matched events

histo::histo() : fib(0),
								red("/home/Li6Webb/Desktop/SFA/caenUnpacker/config/blue_gain_matching.txt", "/home/Li6Webb/Desktop/SFA/caenUnpacker/config/red_gain_matching.txt"),
								blue("/home/Li6Webb/Desktop/SFA/caenUnpacker/config/blue_gain_matching.txt", "/home/Li6Webb/Desktop/SFA/caenUnpacker/config/red_gain_matching.txt") {
  // create root file
  file_read = new TFile("sort.root","RECREATE");
  file_read->cd();

  // create tree
  t = new TTree("t", "t");
	t->Branch("tstamp", &tstamp);
  t->Branch("boardID", &boardID);
  t->Branch("tot", &tot);
  t->Branch("toa", &toa);
	t->Branch("chan", &chan);
  t->Branch("pos", &pos);

	// create matched events tree
	tmatch = new TTree("tmatch", "tmatch");
	tmatch->Branch("fiber", &fib);
	tmatch->Branch("red", &red);
	tmatch->Branch("blue", &blue);

  tot_hist = new TH1F("tot_hist", "Time over Threshold", 1000, 0, 1000);
  toa_hist = new TH1F("toa_hist", "Time of Arrival", 4096, 0, 4096);

  dirHitMap = new TDirectoryFile("FiberHitMap","FiberHitMap");
  dirHitMap->cd();
  Fiber_ixiy = new TH2I("Fiber_ixiy","",64,0,64,64,0,64);
  Fiber_xy = new TH2I("Fiber_xy","",64,-16,16,64,-16,16);
  Fiber_postotx = new TH1F("Fiber_postotx","",64,0,64);
  Fiber_postoty = new TH1F("Fiber_postoty","",64,0,64);
  Fiber_postoax = new TH1F("Fiber_postoax","",2048,0,2048);
  Fiber_postoay = new TH1F("Fiber_postoay","",2048,0,2048);
  Fiber_toax = new TH1I("Fiber_toax","",2048,0,2048);
  Fiber_toay = new TH1I("Fiber_toay","",2048,0,2048);
}

histo::~histo() {
  file_read->Write();
  cout << "file written" << endl;
  file_read->Close();
}

void histo::clear() {
  tot.clear();
  toa.clear();
  chan.clear();
  pos.clear();
  tstamp = -1;
}

void histo::FillTree(double ts, unsigned char id, vector<eventTiming> hits) {
  tstamp = ts;
  boardID = id;
  for (eventTiming hit : hits) {
	  tot.push_back(hit.ToT);
    toa.push_back(hit.ToA);
	  chan.push_back(hit.chan);
	  pos.push_back(hit.pos);
  }
  t->Fill();
}

void histo::FillMatchedTree(fiber* f, Event* r, Event* b) {
	fib = *f;
	red = *r;
	blue = *b;
	tmatch->Fill();
}

#include "histo.h"

// TODO:
// summary spectrum (like Janus)
// same channel multiplicity
// ToA difference between matched events

histo::histo() {
  // create root file
  file_read = new TFile("sort.root","RECREATE");
  file_read->cd();

  // create singles tree
  t = new TTree("t", "t");
	t->Branch("event", &event);

	// create matched events tree
	tmatch = new TTree("tmatch", "tmatch");
	tmatch->Branch("fiber", &fib);
	tmatch->Branch("red", &red);
	tmatch->Branch("blue", &blue);

	// singles histograms
  toa_hist = new TH1I("toa_hist", "Time of Arrival", 4096, 0, 4096);
	tot_summary_blue = new TH2I("tot_summary_blue", "Blue Fibers ToT Summary (gain-matched)", 64, 0, 64, 512, 0, 512);
	tot_summary_red = new TH2I("tot_summary_red", "Red Fibers ToT Summary (gain-matched)", 64, 0, 64, 512, 0, 512);

	// matched events histograms
  dirHitMap = new TDirectoryFile("FiberHitMap","FiberHitMap");
  dirHitMap->cd();
  Fiber_ixiy = new TH2I("Fiber_ixiy","",64,0,64,64,0,64);
  Fiber_xy = new TH2I("Fiber_xy","",64,-16,16,64,-16,16);
	Fiber_tot_summary_x = new TH2I("Fiber_tot_summary_blue", "X Fibers ToT Summary (gain-matched, center hit)", 64, 0, 64, 512, 0, 512);
	Fiber_tot_summary_y = new TH2I("Fiber_tot_summary_red", "Y Fibers ToT Summary (gain-matched, center hit)", 64, 0, 64, 512, 0, 512);
	Fiber_totx = new TH1F("Fiber_totx","",64,-16,16);
  Fiber_toty = new TH1F("Fiber_toty","",64,-16,16);
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
  event.clear();
	red.clear();
	blue.clear();
}

void histo::FillTree(Event e) {
  event = e;
  t->Fill();
}

void histo::FillMatchedTree(fiber* f, Event* r, Event* b) {
	fib = *f;
	red = *r;
	blue = *b;
	tmatch->Fill();
}

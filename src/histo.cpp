#include "histo.h"

histo::histo() {
  // create root file
  file_read = new TFile("sort.root","RECREATE");
  file_read->cd();

  // create tree
  t = new TTree("t", "t");
	t->Branch("tstamp", &tstamp);
  t->Branch("tot", &tot);
  t->Branch("toa", &toa);

  tot_hist = new TH1F("tot_hist", "Time over Threshold", 1000, 0, 1000);
  toa_hist = new TH1F("toa_hist", "Time of Arrival", 4096, 0, 4096);
}

histo::~histo() {
  file_read->Write();
  cout << "file written" << endl;
  file_read->Close();
}

// Extra preparation required for spectroscopy mode
void histo::InitSpecMode() {
	t->Branch("low", &low);
	t->Branch("high", &high);

	lg_hist = new TH1I("lg_hist", "Low Gain", 4096, 0, 4096);
	tot_lg_hist = new TH2F("tot_lg_hist", "Time over Threshold vs. Low Gain", 4096, 0, 4096, 1000, 0, 1000);
}

void histo::FillTree(double ts, unsigned short lg, unsigned short hg, float th, float a) {
	tstamp = ts;
	low = lg;
	high = hg;
  tot = th;
  toa = a;
  t->Fill();
}

void histo::FillTree(double ts, float th, float a) {
  tstamp = ts;
	tot = th;
  toa = a;
  t->Fill();
}

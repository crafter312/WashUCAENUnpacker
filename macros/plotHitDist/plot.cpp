// File created to plot hit distribution for individual event
// Created on 15 August 2024 by Henry Webb (h.s.webb@wustl.edu)

#include "../../src/CAENd5202.h"

#include "TDirectory.h"
#include "TEntryList.h"
#include "TFile.h"
#include "TH1D.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TVirtualPad.h"

#include <exception>
#include <iostream>
#include <string>

#define DATAPATH "/home/Li6Webb/Desktop/SFA/caenUnpacker/RootFiles/Run"

using namespace std;

int main(int argc, char* argv[]) {

	// Get run # from command line arguments
  if (argc != 3) throw invalid_argument("must specify run number and entry offset");
  int runnum = stoi(argv[1]);
	int offset = stoi(argv[2]);
	cout << "Run #: " << runnum << endl;
	cout << "Start at entry # " << offset << endl;

	// Get tree from data file
	string name = string(DATAPATH) + to_string(runnum) + ".root";
	TFile* ifile = new TFile(name.c_str(), "READ");
	TTree* tmatch = ifile->Get<TTree>("tmatch");
	tmatch->SetBranchStatus("fiber", 0);

	// Set up TTreeReader
	tmatch->Draw(">> elist", "red.NHits>4", "entryList");
	TEntryList* elist;
	gDirectory->GetObject("elist", elist);
	TTreeReader treader(tmatch, elist);
	TTreeReaderValue<Event> blue(treader, "blue");
	TTreeReaderValue<Event> red(treader, "red");

	// Setup histogram output
	TFile* ofile = new TFile("plot.root", "RECREATE");
	ofile->cd();
	TH1D* Fiber_totx = new TH1D("Fiber_totx", "", 64, -16, 16);
  TH1D* Fiber_toty = new TH1D("Fiber_toty", "", 64, -16, 16);
  TH1D* Fiber_postotx = new TH1D("Fiber_postotx", "", 64, 0, 64);
  TH1D* Fiber_postoty = new TH1D("Fiber_postoty", "", 64, 0, 64);

	// Event loop
	int count = 0;
	while (treader.Next()) {
		if (count++ < offset) continue;

		Event tempev;
		eventTiming ev;
		double temppos;

		// Blue event
		tempev = *blue;
		for (int k = 0; k < tempev.GetNHits(); k++) {
			ev = tempev.GetTimingEvent(k);
			temppos = -1*(ev.pos-0.5)*0.5 + 16; //mm
			Fiber_totx->AddBinContent(Fiber_totx->GetXaxis()->FindBin(temppos), ev.ToTmatched);
			Fiber_postotx->AddBinContent(Fiber_postotx->GetXaxis()->FindBin(ev.pos), ev.ToTmatched);
		}

		// Red event
		tempev = *red;
		for (int k = 0; k < tempev.GetNHits(); k++) {
			ev = tempev.GetTimingEvent(k);
			temppos = -1*(ev.pos-0.5)*0.5 + 16; //mm
			Fiber_toty->AddBinContent(Fiber_toty->GetXaxis()->FindBin(temppos), ev.ToTmatched);
			Fiber_postoty->AddBinContent(Fiber_postoty->GetXaxis()->FindBin(ev.pos), ev.ToTmatched);
		}

		break;
	}
	cout << "Count: " << count << endl;

	// Cleanup
	ifile->Close();
	ofile->Write();
	ofile->Close();
	delete ofile;
	delete ifile;

}

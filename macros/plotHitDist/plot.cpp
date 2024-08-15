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
	if (argc != 4) throw invalid_argument("must specify run number, entry offset, and multiplicity threshold");
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
	string gate = "red.NHits>" + string(argv[3]);
	cout << "Event selection: " << gate << endl;
	tmatch->Draw(">> elist", gate.c_str(), "entryList");
	TEntryList* elist;
	gDirectory->GetObject("elist", elist);
	cout << "Entries: " << elist->GetN() << endl;
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
		int bin;

		// Blue event
		tempev = *blue;
		for (int k = 0; k < tempev.GetNHits(); k++) {
			ev = tempev.GetTimingEvent(k);
			temppos = -1*(ev.pos-0.5)*0.5 + 16; //mm
			bin = Fiber_totx->GetXaxis()->FindBin(temppos);
			if (Fiber_totx->GetBinContent(bin) > 0)
				cout << "Multiple hit blue fiber found!" << endl;
			Fiber_totx->AddBinContent(bin, ev.ToTmatched);
			Fiber_postotx->AddBinContent(Fiber_postotx->GetXaxis()->FindBin(ev.pos), ev.ToTmatched);
		}

		// Red event
		tempev = *red;
		for (int k = 0; k < tempev.GetNHits(); k++) {
			ev = tempev.GetTimingEvent(k);
			temppos = -1*(ev.pos-0.5)*0.5 + 16; //mm
			bin = Fiber_toty->GetXaxis()->FindBin(temppos);
			if (Fiber_toty->GetBinContent(bin) > 0)
				cout << "Multiple hit red fiber found!" << endl;
			Fiber_toty->AddBinContent(bin, ev.ToTmatched);
			Fiber_postoty->AddBinContent(Fiber_postoty->GetXaxis()->FindBin(ev.pos), ev.ToTmatched);
		}

		break;
	}

	// Cleanup
	ifile->Close();
	ofile->Write();
	ofile->Close();
	delete ofile;
	delete ifile;

}

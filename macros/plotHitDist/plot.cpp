// File created to plot hit distribution for individual event
// Created on 15 August 2024 by Henry Webb (h.s.webb@wustl.edu)

#include "../../src/CAENd5202.h"
#include "../../src/fiber.h"

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

	// Set up TTreeReader
	//string gate = "red.NHits>" + string(argv[3]);
	string gate = "fiber.posmaxhorz!=fiber.posmaxhorznew";
	cout << "Event selection: " << gate << endl;
	tmatch->Draw(">> elist", gate.c_str(), "entryList");
	TEntryList* elist;
	gDirectory->GetObject("elist", elist);
	cout << "Entries: " << elist->GetN() << endl;
	TTreeReader treader(tmatch, elist);
	TTreeReaderValue<fiber> fib(treader, "fiber");
	TTreeReaderValue<Event> blue(treader, "blue");
	TTreeReaderValue<Event> red(treader, "red");

	// Setup histogram output
	TFile* ofile = new TFile("plot.root", "RECREATE");
	ofile->cd();
	TH1D* Fiber_totx = new TH1D("Fiber_totx", "", 64, -16, 16);
	Fiber_totx->SetXTitle("X Fiber Position (mm)");
	TH1D* Fiber_toty = new TH1D("Fiber_toty", "", 64, -16, 16);
	Fiber_toty->SetXTitle("Y Fiber Position (mm)");
	TH1D* Fiber_postotx = new TH1D("Fiber_postotx", "", 64, 0, 64);
	Fiber_postotx->SetXTitle("X Fiber #");
	TH1D* Fiber_postoty = new TH1D("Fiber_postoty", "", 64, 0, 64);
	Fiber_postoty->SetXTitle("Y Fiber #");

	// Event loop
	int count = 0;
	while (treader.Next()) {
		if (count++ < offset) continue;

		fiber tempfib = *fib;
		Event tempev;
		eventTiming ev;
		double temppos;
		int bin;

		// Blue event
		tempev = *blue;
		cout << "Blue multiplicity: " << tempev.GetNHits() << endl;
		cout << "posmaxhorz: " << tempfib.posmaxhorz << endl;
		cout << "posmaxhorz ToT: " << tempev.GetTimingEvent(tempfib.posmaxhorz).ToTmatched << endl;
		cout << "posmaxhorz ToA: " << tempev.GetTimingEvent(tempfib.posmaxhorz).ToA << endl;
		cout << "posmaxhorznew: " << tempfib.posmaxhorznew << endl;
		cout << "posmaxhorznew ToT: " << tempev.GetTimingEvent(tempfib.posmaxhorznew).ToTmatched << endl;
		cout << "posmaxhorznew ToA: " << tempev.GetTimingEvent(tempfib.posmaxhorznew).ToA << endl;
		for (int k = 0; k < tempev.GetNHits(); k++) {
			ev = tempev.GetTimingEvent(k);
			cout << "index: " << k << ", Fiber #: " << ev.pos << ", ToT: " << ev.ToTmatched << ", ToA: " << ev.ToA << endl;
			temppos = -1*(ev.pos-0.5)*0.5 + 16; //mm
			bin = Fiber_totx->GetXaxis()->FindBin(temppos);
			if (Fiber_totx->GetBinContent(bin) > 0)
				cout << "Multiple hit blue fiber found!" << endl;
			Fiber_totx->AddBinContent(bin, ev.ToTmatched);
			Fiber_postotx->AddBinContent(Fiber_postotx->GetXaxis()->FindBin(ev.pos), ev.ToTmatched);
		}

		// Red event
		tempev = *red;
		cout << "Red multiplicity: " << tempev.GetNHits() << endl;
		cout << "posmaxvert: " << tempfib.posmaxvert << endl;
		cout << "posmaxvert ToT: " << tempev.GetTimingEvent(tempfib.posmaxvert).ToTmatched << endl;
		cout << "posmaxvertnew: " << tempfib.posmaxvertnew << endl;
		cout << "posmaxvertnew ToT: " << tempev.GetTimingEvent(tempfib.posmaxvertnew).ToTmatched << endl;
		for (int k = 0; k < tempev.GetNHits(); k++) {
			ev = tempev.GetTimingEvent(k);
			cout << "index: " << k << ", Fiber #: " << ev.pos << ", ToT: " << ev.ToTmatched << ", ToA: " << ev.ToA << endl;
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

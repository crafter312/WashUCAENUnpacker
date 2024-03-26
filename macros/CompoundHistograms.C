/**
 * File created by Henry Webb on 15 February 2024.
 * Input files must be created by unpacker and copied/renamed to "run_[run # here].root".
 * Input files must be located in the same directory from which this macro is run.
 */

#include <stdarg.h>
#include <string>

#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TTree.h"

// Draw background-subtracted histogram
void NoBackgroundHist(int source, int background, EColor color, bool same = false, bool highGain = false) {

	// Define strings and constants
	const char* histTitle = highGain ? "Background-subtracted High Gain" : "Background-subtracted Time Over Threshold";
	const char* xAxis = highGain ? "High Gain" : "ToT (ns)";
	const char* gates = highGain ? "" : "tot > -1";
	const char* opts = same ? "SAME" : "";
	const int bins[2]  = {50, 120};
	const int xlow[2]  = {0, 60};
	const int xhigh[2] = {50, 660};

	// Make histogram
	string fName = "run_" + to_string(source) + ".root";
	string histDraw1 = (highGain ? "high>>" : "tot>>") + fName;
	TFile* f1 = new TFile(fName.c_str(), "READ");
	TTree* t1 = (TTree*)f1->Get("t");
	TH1F* h1 = new TH1F(fName.c_str(), histTitle, bins[highGain], xlow[highGain], xhigh[highGain]);
	h1->SetLineColor(color);
	t1->Draw(histDraw1.c_str(), gates, "goff");

	fName = "run_" + to_string(background) + ".root";
	string histDraw1b = (highGain ? "high>>" : "tot>>") + fName;
	TFile* f1b = new TFile(fName.c_str(), "READ");
	TTree* t1b = (TTree*)f1b->Get("t");
	TH1F* h1b = new TH1F(fName.c_str(), histTitle, bins[highGain], xlow[highGain], xhigh[highGain]);
	t1b->Draw(histDraw1b.c_str(), gates, "goff");

	h1->Add(h1b, -1);
	if(!same) SetAxisLabels(h1, xAxis, "Counts");
	h1->Draw(opts);
}

// Overlays two ToT histograms in a nice plot
void OverlayToT(bool highGain = false) {

	TCanvas *C = new TCanvas();
	gStyle->SetPalette(91);
	gStyle->SetOptStat(0);

	NoBackgroundHist(50, 54, kBlue); // AnalogProbe0: Off
	NoBackgroundHist(73, 74, kRed, true); // AnalogProbe0: FAST
	gPad->BuildLegend();
}

void ToTvHighGainSingle(int source, int background) {

	const char* histTitle = "Background-subtracted ToT vs. High Gain";
	const char* gates = "tot > -1";

	string fName = "run_" + to_string(source) + ".root";
	string histDraw1 = "tot:high>>" + fName;
	TFile* f1 = new TFile(fName.c_str(), "READ");
	TTree* t1 = (TTree*)f1->Get("t");
	TH2F* h1 = new TH2F(fName.c_str(), histTitle, 100, 100, 500, 50, 0, 50);
	t1->Draw(histDraw1.c_str(), gates, "goff");

	fName = "run_" + to_string(background) + ".root";
	histDraw1 = "tot:high>>" + fName;
	TFile* f1b = new TFile(fName.c_str(), "READ");
	TTree* t1b = (TTree*)f1b->Get("t");
	TH2F* h1b = new TH2F(fName.c_str(), histTitle, 100, 100, 500, 50, 0, 50);
	t1b->Draw(histDraw1.c_str(), gates, "goff");

	h1->Add(h1b, -1);
	SetAxisLabels(h1, "High Gain", "ToT (ns)");
	h1->Draw("colz");
	gPad->BuildLegend();
}

void ToTvHighGain() {

	gStyle->SetOptStat(0);

	// AnalogProbe0: Off
	TCanvas *c = new TCanvas();
	ToTvHighGainSingle(50, 54);

	// AnalogProbe0: FAST
	TCanvas *c1 = new TCanvas();
	ToTvHighGainSingle(73, 74);
}

void ToTlogFit(int source, int background, string probe) {
	const char* histTitle = "Background-subtracted ToT vs. # of Photoelectrons";
	const char* gates = "tot > -1";

	string fName = "run_" + to_string(source) + ".root";
	string histDraw1 = "tot:0.0239*high-3.0075>>" + fName;
	TFile* f1 = new TFile(fName.c_str(), "READ");
	TTree* t1 = (TTree*)f1->Get("t");
	TH2F* h1 = new TH2F(fName.c_str(), histTitle, 100, 0, 10, 50, 0, 50);
	t1->Draw(histDraw1.c_str(), gates, "goff");

	fName = "run_" + to_string(background) + ".root";
	histDraw1 = "tot:0.0239*high-3.0075>>" + fName;
	TFile* f1b = new TFile(fName.c_str(), "READ");
	TTree* t1b = (TTree*)f1b->Get("t");
	TH2F* h1b = new TH2F(fName.c_str(), histTitle, 100, 0, 10, 50, 0, 50);
	t1b->Draw(histDraw1.c_str(), gates, "goff");

	h1->Add(h1b, -1);
	SetAxisLabels(h1, "# of Photoelectrons", "ToT (ns)");
	h1->SetStats(false);
	h1->Draw("colz");

	string title = "AnalogProbe0: " + probe;
	h1->SetTitle(title.c_str());
	gPad->BuildLegend();
	h1->SetTitle(histTitle);
	
	TF1 *ft1 = new TF1("ft1","[0]*TMath::Log(x/[1])",0,1);
	ft1->SetParameters(15,0.2);
	ft1->SetLineColor(kRed);
	h1->Fit(ft1);
	ft1->Draw("same");
}

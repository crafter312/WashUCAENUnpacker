/**
 * File created by Henry Webb on 18 January 2024.
 * Input files must be created by unpacker and copied/renamed to "run_[run # here].root".
 * Input files must be located in the same directory from which this macro is run.
 * Macro inputs:
 * 		runAll: integer run number for data with source close to fiber
 * 		runNoise: integer run number for data with source away from fiber
 * 		diffOnly: if true, this bypasses all fitting steps and only graphs the histogram difference
 * 		highGain: if true, uses the high gain parameter instead of the time over threshold
 * 		ignoreFirstPoint: if true, this ignores the first photo-electron point during the Poisson fit
 * 		ignoreLastPoint: if true, this ignores the last photo-electron point during the Poisson fit
 */

#include <algorithm>
#include <map>
#include <stdexcept>
#include <utility>
#include <vector>

#include "TAxis.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TF1.h"
#include "TGraph.h"
#include "TH1.h"
#include "TLegend.h"
#include "TPaveStats.h"
#include "TTree.h"

using namespace std;

void SubtractHists(const int& runAll, const int& runNoise, bool diffOnly = false, bool highGain = true, bool ignoreFirstPoint = false, bool ignoreLastPoint = false) {

	TCanvas* c = new TCanvas("c", "c");
	c->cd();

	// Conditional Graph Titles
	const char* histTitle = highGain ? "High Gain" : "Time Over Threshold";
	const char* xAxis = highGain ? "High Gain" : "ToT (ns)";
	const char* allHist = highGain ? "high>>all" : "tot>>all";
	const char* noiseHist = highGain ? "high>>noise" : "tot>>noise";
	const char* legendTitle = highGain ? "HG Runs" : "ToT Runs";
	const char* histTitleSub = highGain ? "High Gain (noise subtracted);High Gain;Counts" : "Time Over Threshold (noise subtracted);ToT (ns);Counts";

	const int bins[2]  = {50, 120};
	const int xlow[2]  = {0, 60};
	const int xhigh[2] = {50, 660};

	// Make histograms
	string allName = "run_" + to_string(runAll) + ".root";
	TFile* allFile = new TFile(allName.c_str(), "READ");
	TTree* allTree = (TTree*)allFile->Get("t");
	TH1F* all = new TH1F("all",histTitle,bins[highGain],xlow[highGain],xhigh[highGain]);
	gStyle->SetOptStat(0000);
	all->SetLineColor(kRed);
	SetAxisLabels(all, xAxis, "Counts");
	allTree->Draw(allHist);

	string noiseName = "run_" + to_string(runNoise) + ".root";
	TFile* noiseFile = new TFile(noiseName.c_str(), "READ");
	TTree* noiseTree = (TTree*)noiseFile->Get("t");
	TH1F* noise = new TH1F("noise",histTitle,bins[highGain],xlow[highGain],xhigh[highGain]);
	noiseTree->Draw(noiseHist,"","same");

	TLegend* legend = new TLegend(0.6, 0.7, 0.9, 0.9);
	legend->SetHeader(legendTitle, "C");
	string legNameAll = "Run " + to_string(runAll) + " (source)";
	string legNameNoise = "Run " + to_string(runNoise) + " (no source)";
	legend->AddEntry(all, legNameAll.c_str(), "L");
	legend->AddEntry(noise, legNameNoise.c_str(), "L");
	legend->Draw();

	// Check histogram properties
	int numBinsXall = all->GetNbinsX();
	int numBinsXnoise = noise->GetNbinsX();
	const TAxis* xAxisAll = all->GetXaxis();
	const TAxis* xAxisNoise = noise->GetXaxis();
	if(numBinsXall != numBinsXnoise)
		throw invalid_argument("Supplied histograms contain different numbers of bins!");
	else if(xAxisAll->GetXmin() != xAxisNoise->GetXmin() || 
					xAxisAll->GetXmax() != xAxisNoise->GetXmax()) 
		throw invalid_argument("Supplied histograms contain different ranges!");

	// Retrieve bin counts from histograms
	double binEdge[numBinsXall];
	double binDiff[numBinsXall];
	int i2;
	for(int i = 0; i < numBinsXall; i++) {
		i2 = i + 1;
		binEdge[i] = all->GetBinLowEdge(i2);
		binDiff[i] = all->GetBinContent(i2) - noise->GetBinContent(i2);
	}

	// Draw difference of histograms
	TCanvas* c1 = new TCanvas("c1", "c1");
	c1->cd();
	TGraph* graph = new TGraph(numBinsXall, binEdge, binDiff);
	graph->SetTitle(histTitleSub);
	TAxis* axis = graph->GetXaxis();
	axis->CenterTitle();
	axis->SetTitleOffset(0.7);
	axis->SetTitleSize(0.06);
	axis = graph->GetYaxis();
	axis->SetRangeUser(0, axis->GetXmax());
	axis->CenterTitle();
	axis->SetTitleOffset(0.8);
	axis->SetTitleSize(0.06);
	graph->Draw("AC*");

	// Return if only viewing the histogram difference
	if(diffOnly == true) return;

	//******** Multi-Gaussian fit ********//

	// Define run pairs (source, background)
	vector<pair<int, int>> runs;
	runs.push_back(make_pair(32, 33));
	runs.push_back(make_pair(36, 37));
	runs.push_back(make_pair(42, 47));
	runs.push_back(make_pair(41, 46));
	runs.push_back(make_pair(40, 45));
	runs.push_back(make_pair(38, 39));
	runs.push_back(make_pair(43, 44));
	runs.push_back(make_pair(75, 76));
	runs.push_back(make_pair(50, 54));

	// Check supplied run numbers
	pair<int, int> currentRuns(runAll, runNoise);
	if(find(runs.begin(), runs.end(), currentRuns) == runs.end())
		throw invalid_argument("Run pair has no associated initial fit parameters!");

	// Define run-specific parameters
	map<pair<int, int>, vector<double>> amplitudes;
	amplitudes[runs[0]] = {100, 100, 100, 50, 25};
	amplitudes[runs[1]] = {2000, 2000, 2000, 2000, 1000, 600, 300, 150};
	amplitudes[runs[2]] = {50, 20, 5};
	amplitudes[runs[3]] = {218, 300, 180, 85, 45, 22};
	amplitudes[runs[4]] = {215, 500, 400, 240, 150, 100, 30};
	amplitudes[runs[5]] = {1500, 1500, 1500, 1000, 600, 300, 150, 100};
	amplitudes[runs[6]] = {490, 500, 450, 300, 200, 150, 70, 50};
	amplitudes[runs[7]] = {800, 500, 400, 300, 300, 200, 150, 70, 60};
	amplitudes[runs[8]] = {800, 500, 400, 300, 300, 200, 150};

	map<pair<int, int>, vector<double>> centroids;
	centroids[runs[0]] = {7, 12, 17, 21, 25};
	centroids[runs[1]] = {9, 14.5, 19, 23, 28, 33, 36, 40};
	centroids[runs[2]] = {175, 210, 230};
	centroids[runs[3]] = {163, 190, 214, 237, 275, 297};
	centroids[runs[4]] = {157, 190, 220, 255, 290, 330, 365};
	centroids[runs[5]] = {177, 217, 257, 300, 345, 388, 435, 478};
	centroids[runs[6]] = {190, 235, 283, 335, 385, 435, 484, 540};
	centroids[runs[7]] = {155, 215, 273, 332, 395, 452, 510, 565, 628};
	centroids[runs[8]] = {170, 210, 250, 290, 335, 375, 420};

	map<pair<int, int>, vector<double>> sigmas;
	sigmas[runs[0]] = {1.292, 1.292, 1.292, 1.292, 1.292};
	sigmas[runs[1]] = {1.292, 1.292, 1.292, 1.292, 1.292, 1.292, 1.292, 1.292};
	sigmas[runs[2]] = {1.292, 1.292, 1.292};
	sigmas[runs[3]] = {9, 9, 10, 10, 8, 10};
	sigmas[runs[4]] = {10, 10, 10, 8, 10, 10, 10};
	sigmas[runs[5]] = {10, 10, 10, 10, 10, 10, 10, 10};
	sigmas[runs[6]] = {10, 10, 10, 10, 10, 10, 10, 10};
	sigmas[runs[7]] = {10, 10, 10, 10, 10, 10, 10, 10, 10};
	sigmas[runs[8]] = {10, 10, 10, 10, 10, 10, 10};

	// Construct fit equation
	string fitEq = "";
	const int n = min(min(amplitudes[currentRuns].size(), centroids[currentRuns].size()), sigmas[currentRuns].size());
	string a, b, d;
	for(int i = 0; i < n; i++) {
		a = "[" + to_string((i * 3) + 0) + "]"; // constant
		b = "[" + to_string((i * 3) + 1) + "]"; // mean
		d = "[" + to_string((i * 3) + 2) + "]"; // sigma
		fitEq += a + "*TMath::Gaus(x," + b + "," + d + ")";
		if(i == n - 1) continue;
		fitEq += "+";
	}

	cout << "Multi-Gaussian Fit:\n" << fitEq << endl;

	// Set fit parameters
	TF1* multiGaus = new TF1("multiGaus", fitEq.c_str(), binEdge[0], binEdge[numBinsXall - 1]);
	multiGaus->SetRange(binEdge[0], binEdge[numBinsXall - 1]);
	vector<double> amplitude = amplitudes[currentRuns];
	vector<double> centroid  = centroids[currentRuns];
	vector<double> sigma     = sigmas[currentRuns];
	int e, f, g;
	for(int i = 0; i < n; i++) {
		e = (i * 3) + 0;
		f = (i * 3) + 1;
		g = (i * 3) + 2;
		multiGaus->SetParameter(e, amplitude[i]);
		multiGaus->SetParameter(f, centroid[i]);
		multiGaus->SetParameter(g, sigma[i]);
		multiGaus->SetParLimits(e, 0, 10000);
		multiGaus->SetParLimits(f, centroid[i]-3, centroid[i]+3);
		multiGaus->SetParLimits(g, 0.9, highGain ? 21 : 10);
		a = "Constant " + to_string(i);
		b = "Mean " + to_string(i);
		d = "Sigma " + to_string(i);
		multiGaus->SetParName(e, a.c_str());
		multiGaus->SetParName(f, b.c_str());
		multiGaus->SetParName(g, d.c_str());
	}
	//multiGaus->SetLineColor(kBlue);
	//TF1* multiGausCopy = multiGaus->DrawCopy("CSAME");

	// Fit and integrate peaks
	multiGaus->SetLineColor(kRed);
	graph->Fit(multiGaus, "NR", "", binEdge[0], binEdge[numBinsXall - 1]);
	multiGaus->DrawCopy("CSAME");

	double nPhotons[n];
	double integral[n];
	double mean[n];
	TF1* gaus = new TF1("gausFit", "gaus", binEdge[0], binEdge[numBinsXall - 1]);
	gaus->SetLineColor(kBlue);
	gaus->SetLineStyle(2);

	TLegend* legend2 = new TLegend(0.6, 0.75, 0.9, 0.9);
	//legend2->AddEntry(multiGausCopy, "Initial Parameters", "L");
	legend2->AddEntry(multiGaus, "Fit Result", "L");
	legend2->AddEntry(gaus, "Individual Peaks", "L");
	legend2->Draw();

	for(int i = 0; i < n; i++) {
		nPhotons[i] = i + 1;
		gaus->SetParameter(0, multiGaus->GetParameter((i * 3) + 0));
		gaus->SetParameter(1, multiGaus->GetParameter((i * 3) + 1));
		gaus->SetParameter(2, multiGaus->GetParameter((i * 3) + 2));
		integral[i] = gaus->Integral(binEdge[0], binEdge[numBinsXall - 1]);
		mean[i] = multiGaus->GetParameter((i * 3) + 1);
		gaus->DrawCopy("CSAME");
	}

	// Draw integral vs. number of photons
	TCanvas* c2 = new TCanvas("c2", "c2");
	c2->cd();
	TGraph* graph2 = new TGraph(n, nPhotons, integral);
	graph2->SetTitle("Counts Per # Photo-electrons;# Photo-electrons;Counts");
	axis = graph2->GetXaxis();
	axis->CenterTitle();
	axis->SetTitleOffset(0.7);
	axis->SetTitleSize(0.06);
	axis = graph2->GetYaxis();
	axis->CenterTitle();
	axis->SetTitleOffset(0.8);
	axis->SetTitleSize(0.06);
	graph2->Draw("AC*");

	// Fit with Poisson distribution
	TF1* fit = new TF1("fit", "[0]*TMath::Poisson(x,[1])", nPhotons[ignoreFirstPoint], nPhotons[n - 1 - ignoreLastPoint]);
	fit->SetRange(nPhotons[ignoreFirstPoint], nPhotons[n - 1 - ignoreLastPoint]);
	fit->SetParName(0, "Amplitude");
	fit->SetParName(1, "Expectation");
	fit->SetParameter(0, 1000);
	fit->SetParameter(1, 0.5);
	graph2->Fit(fit, "R", "C", nPhotons[ignoreFirstPoint], nPhotons[n - 1 - ignoreLastPoint]);
	gStyle->SetOptFit(0011);
	graph2->PaintStats(fit);
	TPaveStats *st = (TPaveStats*)graph2->FindObject("stats");
	st->SetX1NDC(0.6);
	st->SetX2NDC(0.9);
	st->SetY1NDC(0.7);
	st->SetY2NDC(0.9);

	// Spacing of first two peaks
	cout << "Spacing of first two peaks: " << mean[1] - mean[0] << endl;
}

#include <algorithm>
#include <iomanip>
#include <stdexcept>

// Create default variables
{
	string DATAPATH = "/home/Li6Webb/Desktop/SFA/caenUnpacker/RootFiles/Run";

	gPad->SetTickx();
	gPad->SetTicky();
}

void SetAliases(TChain& ch) {
	// Common aliases
	ch.SetAlias("t_layer_diff", "TMath::Abs(blue.dataTiming[fiber.posmaxhorz].ToA-red.dataTiming[fiber.posmaxvert].ToA)<20");

	// Blue fiber aliases
	ch.SetAlias("t_center_hit_blue", "(blue.dataTiming[fiber.posmaxhorz].ToA)>650 && (blue.dataTiming[fiber.posmaxhorz].ToA)<740");
	ch.SetAlias("t_hit_diff_blue", "(blue.dataTiming.ToA-blue.dataTiming[fiber.posmaxhorz].ToA)>=0 && (blue.dataTiming.ToA-blue.dataTiming[fiber.posmaxhorz].ToA)<50");

	// Red fiber aliases
	ch.SetAlias("t_center_hit_red", "(red.dataTiming[fiber.posmaxvert].ToA)>650 && (red.dataTiming[fiber.posmaxvert].ToA)<740");
	ch.SetAlias("t_hit_diff_red", "(red.dataTiming.ToA-red.dataTiming[fiber.posmaxvert].ToA)>=0 && (red.dataTiming.ToA-red.dataTiming[fiber.posmaxvert].ToA)<100");
}

void FillScaledHitToT(TChain& ch, string histname, int i, int boardID, bool abs = false) {
	abs = (i == 0) ? false : abs;
	string varexp = boardID == 1
		? "blue.dataTiming.ToTmatched/fiber.sumhorz>>" + histname
		: "red.dataTiming.ToTmatched/fiber.sumvert>>" + histname;
	string selection = boardID == 1
		? ("(blue.dataTiming.pos-fiber.ix)==" + to_string(i) + (abs ? " || (blue.dataTiming.pos-fiber.ix)==" + to_string(i*-1) : ""))
		: ("(red.dataTiming.pos-fiber.iy)==" + to_string(i) + (abs ? " || (red.dataTiming.pos-fiber.iy)==" + to_string(i*-1) : ""));
	ch.Draw(varexp.c_str(),selection.c_str(),"goff");
}

void ScaledHitToT(vector<int> nums, int boardID = 1, int i = 0, bool abs = false, string newtitle = "") {
	TChain chain("tmatch");
	string name;
	for (int i = 0; i < nums.size(); i++) {
		name = DATAPATH + to_string(nums[i]) + ".root";
		chain.Add(name.c_str());
	}

	string defaulttitle = "% ToT " + to_string(i) + " Hit" + (i == 1 ? "" : "s") + " from Center";
	string title = ((newtitle == "") ? defaulttitle : newtitle) + ";Fraction of Total ToT;Counts per Bin";

	TH1I* h = new TH1I("h", "h", 128, 0, 1);
	FillScaledHitToT(chain, h->GetName(), i, boardID, abs);
	h->SetTitle(title.c_str());
	h->SetStats(0);

	TAxis* ax = h->GetXaxis(); 
	ax->CenterTitle();
	ax->SetTitleSize(0.06);
	ax->SetTitleOffset(0.66);

	ax = h->GetYaxis(); 
	ax->CenterTitle();
	ax->SetTitleSize(0.06);
	ax->SetTitleOffset(0.78);
	
	h->Draw();
}

void LightLeakage(vector<int> nums, int boardID = 1) {
	TChain chain("tmatch");
	string name;
	for (int i = 0; i < nums.size(); i++) {
		name = DATAPATH + to_string(nums[i]) + ".root";
		chain.Add(name.c_str());
	}

	TH1D* percentHist = new TH1D("percentHist", "Percent Light in Fiber", 4, 0, 4);
	for (int i = 0; i < 4; i++) {
		TH1I* h0 = new TH1I("h0", "h0", 128, 0, 1);
		FillScaledHitToT(chain, h0->GetName(), i, boardID, i != 0);
		percentHist->Fill(i, h0->GetMean());
		delete h0;
	}

	percentHist->Draw();
}

void HitMultFit(vector<int> nums, int boardID = 1, int max = 12) {
	TChain chain("tmatch");
	string name;
	for (int i = 0; i < nums.size(); i++) {
		name = DATAPATH + to_string(nums[i]) + ".root";
		chain.Add(name.c_str());
	}
	SetAliases(chain);

	string title = boardID == 1
		? "Blue Fiber Hit Multiplicity"
		: "Red Fiber Hit Multiplicity";
	TH1I* hitMult = new TH1I("hitMult", title.c_str(), max, 0, max);

	string varexp = boardID == 1
		? "blue.NHits"
		: "red.NHits";
	varexp += ">>hitMult";
	string selection = boardID == 1
		? "t_center_hit_blue"
		: "t_center_hit_red";
	selection += " && t_layer_diff";
	chain.Draw(varexp.c_str(), selection.c_str(), "goff");

	// Fit with Poisson distribution
	TF1* fit = new TF1("fit", "gaus", 0, max);
	fit->SetRange(0, max);
	hitMult->Fit(fit, "R", "C", 0, max);
	gStyle->SetOptFit(0011);
	hitMult->Draw();
	
	// Set stats location
	TPaveStats *st = (TPaveStats*)hitMult->FindObject("stats");
	st->SetX1NDC(0.6);
	st->SetX2NDC(0.9);
	st->SetY1NDC(0.5);
	st->SetY2NDC(0.9);
	st->Draw();
}

void HitDist(vector<int> nums, int boardID = 1, int ymax = 1) {
	TChain chain("tmatch");
	string name;
	for (int i = 0; i < nums.size(); i++) {
		name = DATAPATH + to_string(nums[i]) + ".root";
		chain.Add(name.c_str());
	}
	SetAliases(chain);

	string title = boardID == 1
		? "Blue Fiber Hit Map"
		: "Red Fiber Hit Map";
	TH2I* hitMap = new TH2I("hitMap", title.c_str(), 128, -64, 64, 512, 0, ymax);

	string varexp = boardID == 1
		? "blue.dataTiming.ToTmatched/fiber.sumhorz:blue.dataTiming.pos-fiber.ix"
		: "red.dataTiming.ToTmatched/fiber.sumvert:red.dataTiming.pos-fiber.iy";
	varexp += ">>hitMap";
	string selection = boardID == 1
		? "t_center_hit_blue && t_hit_diff_blue"
		: "t_center_hit_red && t_hit_diff_red && red.NHits<=4";
	selection += " && t_layer_diff";
	chain.Draw(varexp.c_str(), selection.c_str(), "goff");
	hitMap->Draw("colz");
	gPad->Update();

	// Set stats location
	TPaveStats *st = (TPaveStats*)hitMap->FindObject("stats");
	st->SetX1NDC(0.6);
	st->SetX2NDC(0.9);
	st->SetY1NDC(0.5);
	st->SetY2NDC(0.9);
	st->Draw();
}

// For use with gain matching, plots the center hit ToT for a specific fiber and fits
// it with a Gaussian distribution
//	- nums: a list of run numbers to merge into a TChain object
//	- fibernum: the number of fiber to plot and fit
//	- boardID: the relevant board number; defaults to board 1 (blue/front)
//	- mult: the maximum hit multiplicity of events being considered
//	- dxl: the left range of the fit
//	- dxr: the right range of the fit
void ToTPeakFit(vector<int> nums, int fibernum, int boardID = 1, int mult = 1, double dxl = 5, double dxr = 3) {
	TChain chain("tmatch");
	string name;
	for (int i = 0; i < nums.size(); i++) {
		name = DATAPATH + to_string(nums[i]) + ".root";
		chain.Add(name.c_str());
	}

	string title = boardID == 1
		? "Blue Fiber Center Hit ToT (fiber "
		: "Red Fiber Center Hit ToT (fiber ";
	title += to_string(fibernum) + string(")");

	cout << "searching for histogram..." << endl;
	TH1I* hist = nullptr;
	TList* primitives = gPad->GetListOfPrimitives(); 
	for (TObject* obj: *primitives) {
		if (TH1* v = dynamic_cast<TH1*>(obj)) {
			string objName = string(obj->GetName());
			if (objName == "tot") {
				hist = dynamic_cast<TH1I*>(v);
				cout << "histogram found!" << endl;
			}
		}
	}
	if (hist == nullptr) {
		hist = new TH1I("tot", title.c_str(), 512, 0, 512);
		cout << "histogram not found, making new one!" << endl;
	}
	else
		hist->SetTitle(title.c_str());

	string varexp = boardID == 1
		? "blue.dataTiming[fiber.posmaxhorz].ToT"
		: "red.dataTiming[fiber.posmaxvert].ToT";
	varexp += ">>tot";
	string selection = boardID == 1
		? "fiber.ix=="
		: "fiber.iy==";
	string multGate = boardID == 1
		? "blue.NHits<="
		: "red.NHits<=";
	selection += to_string(fibernum) + " && " + multGate + to_string(mult);
	chain.Draw(varexp.c_str(), selection.c_str(), "goff");
	hist->Draw("");

	double centroid = hist->GetMaximumBin();
	cout << "Bin with maximum value: " << centroid << endl;

	TF1* gaus = new TF1("gaus", "gaus", centroid-dxl, centroid+dxr);
	gaus->SetRange(centroid-dxl, centroid+dxr);
	gaus->SetParameter(0, hist->GetMaximum());
	gaus->SetParameter(1, centroid);
	gaus->SetParameter(2, 10);
	gaus->SetParLimits(0, 0, 100000);
	gaus->SetParLimits(1, centroid-dxl, centroid+dxr);
	gaus->SetParLimits(2, 0, 30);
	gaus->SetParName(0, "Constant");
	gaus->SetParName(1, "Mean");
	gaus->SetParName(2, "Sigma");

	gaus->SetLineColor(kRed);
	hist->Fit(gaus, "NR", "", centroid-dxl, centroid+dxr);
	gaus->DrawCopy("CSAME");

	cout << "Centroid: " << setprecision(10) << gaus->GetParameter(1) << endl;
	cout << "Sigma: " << setprecision(10) << gaus->GetParameter(2) << endl;
}

// Chain multiple files and make summary plot
void ChainedSummary(vector<int> nums, int boardID = 1) {
	TChain chain("tmatch");
	string name;
	for (int i = 0; i < nums.size(); i++) {
		name = DATAPATH + to_string(nums[i]) + ".root";
		chain.Add(name.c_str());
	}

	string varexp = boardID == 1
		? "blue.dataTiming.ToTmatched:blue.dataTiming.pos"
		: "red.dataTiming.ToTmatched:red.dataTiming.pos";
	varexp += ">>h(64,0,64,512,0,512)";
	chain.Draw(varexp.c_str(), "", "colz");
}




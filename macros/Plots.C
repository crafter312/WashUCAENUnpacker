#include <stdexcept>

// Create default variables
{
	TFile* ifile = new TFile();
	int num = -1;

	gPad->SetTickx();
	gPad->SetTicky();
}

void OpenFile(int runnum) {
	if (ifile->IsOpen() && (runnum == num)) return;
	
	num = runnum;
	string name = "/home/Li6Webb/Desktop/SFA/caenUnpacker/RootFiles/Run" + to_string(runnum) + ".root";
	ifile = new TFile(name.c_str(), "READ");
}

void SetAliases(TTree* t) {
	// Common aliases
	t->SetAlias("t_layer_diff", "TMath::Abs(blue.dataTiming[fiber.posmaxhorz].ToA-red.dataTiming[fiber.posmaxvert].ToA)<20");

	// Blue fiber aliases
	t->SetAlias("t_center_hit_blue", "(blue.dataTiming[fiber.posmaxhorz].ToA)>650 && (blue.dataTiming[fiber.posmaxhorz].ToA)<740");
	t->SetAlias("t_hit_diff_blue", "(blue.dataTiming.ToA-blue.dataTiming[fiber.posmaxhorz].ToA)>=0 && (blue.dataTiming.ToA-blue.dataTiming[fiber.posmaxhorz].ToA)<50");

	// Red fiber aliases
	t->SetAlias("t_center_hit_red", "(red.dataTiming[fiber.posmaxvert].ToA)>650 && (red.dataTiming[fiber.posmaxvert].ToA)<740");
	t->SetAlias("t_hit_diff_red", "(red.dataTiming.ToA-red.dataTiming[fiber.posmaxvert].ToA)>=0 && (red.dataTiming.ToA-red.dataTiming[fiber.posmaxvert].ToA)<100");
}

void FillScaledHitToT(TTree* t, string histname, int i, int boardID, bool abs = false) {
	abs = (i == 0) ? false : abs;
	string varexp = boardID == 1
		? "blue.dataTiming.ToTmatched/fiber.sumhorz>>" + histname
		: "red.dataTiming.ToTmatched/fiber.sumvert>>" + histname;
	string selection = boardID == 1
		? ("(blue.dataTiming.pos-fiber.ix)==" + to_string(i) + (abs ? " || (blue.dataTiming.pos-fiber.ix)==" + to_string(i*-1) : ""))
		: ("(red.dataTiming.pos-fiber.iy)==" + to_string(i) + (abs ? " || (red.dataTiming.pos-fiber.iy)==" + to_string(i*-1) : ""));
	t->Draw(varexp.c_str(),selection.c_str(),"goff");
}

void ScaledHitToT(int runnum, int boardID = 1, int i = 0, bool abs = false, string newtitle = "") {
	OpenFile(runnum);
	TTree* tmatch = ifile->Get<TTree>("tmatch");

	string defaulttitle = "% ToT " + to_string(i) + " Hit" + (i == 1 ? "" : "s") + " from Center";
	string title = ((newtitle == "") ? defaulttitle : newtitle) + ";Fraction of Total ToT;Counts per Bin";

	TH1I* h = new TH1I("h", "h", 128, 0, 1);
	FillScaledHitToT(tmatch, h->GetName(), i, boardID, abs);
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

void LightLeakage(int runnum, int boardID = 1) {
	OpenFile(runnum);
	TTree* tmatch = ifile->Get<TTree>("tmatch");

	TH1D* percentHist = new TH1D("percentHist", "Percent Light in Fiber", 4, 0, 4);
	for (int i = 0; i < 4; i++) {
		TH1I* h0 = new TH1I("h0", "h0", 128, 0, 1);
		FillScaledHitToT(tmatch, h0->GetName(), i, boardID, i != 0);
		percentHist->Fill(i, h0->GetMean());
		delete h0;
	}

	percentHist->Draw();
}

void HitMultFit(int runnum, int boardID = 1, int max = 12) {
	OpenFile(runnum);
	TTree* tmatch = ifile->Get<TTree>("tmatch");
	SetAliases(tmatch);

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
	tmatch->Draw(varexp.c_str(), selection.c_str(), "goff");

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

void HitMap(int runnum, int boardID = 1, int ymax = 1) {
	OpenFile(runnum);
	TTree* tmatch = ifile->Get<TTree>("tmatch");
	SetAliases(tmatch);

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
		: "t_center_hit_red && t_hit_diff_red";
	selection += " && t_layer_diff";
	tmatch->Draw(varexp.c_str(), selection.c_str(), "goff");
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

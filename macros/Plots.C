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

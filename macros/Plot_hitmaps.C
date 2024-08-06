void Plot_hitmaps(int run = 0) {
	gStyle->SetOptStat(0);

	TFile *infile;
	if (run == 0)
		infile = new TFile("/home/Li6Webb/Desktop/SFA/caenUnpacker/sort.root");
	else
		infile = new TFile(Form("/home/Li6Webb/Desktop/SFA/caenUnpacker/RootFiles/Run%i.root",run));	 

	if (!infile->IsOpen()) {
		cout << "Couldn't open file" << endl;
		return;
	}

	TCanvas *can1 = new TCanvas("can1", "", 1000, 300);
	can1->Draw();

	TH1F *xmap = (TH1F*)infile->Get("FiberHitMap/Fiber_totx");
	xmap->SetTitle("Blue Fibers Example Event (21 MeV/u alphas)");
	xmap->Draw();

	TCanvas *can2 = new TCanvas("can2", "", 1000, 300);
	can2->Draw();

	TH1F *ymap = (TH1F*)infile->Get("FiberHitMap/Fiber_toty");
	ymap->SetTitle("Red Fibers Example Event (21 MeV/u alphas)");

	TAxis* xaxis = ymap->GetXaxis();
	for (int i = 0; i < 8; i++)
		xaxis->ChangeLabel(i, 270, 0.06);

	TAxis* yaxis = ymap->GetYaxis();
	for (int i = 0; i < 8; i++)
		yaxis->ChangeLabel(i, 270, 0.06);

	ymap->Draw();
}

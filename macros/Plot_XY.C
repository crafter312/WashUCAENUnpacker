void Plot_XY(int run = 0) {
	//gROOT->SetStyle("Plain");
	gStyle->SetPalette(kBird);
	gStyle->SetOptStat(0);

	TFile *infile;
	if(run==0)
		infile = new TFile("/home/Li6Webb/Desktop/SFA/caenUnpacker/sort.root");
	else
		infile = new TFile(Form("/home/Li6Webb/Desktop/SFA/caenUnpacker/RootFiles/Run%i.root",run));	 

	if(!infile->IsOpen()) {
		cout << "Couldn't open file" << endl;
		return;
	}

	// Calculate fractional margins (default margin is 0.1 fraction of width/height)
	int canw = 900;
	int canh = 800;
	int pixr = 100; // extra right margin in pixels

	int histw = (int)(canw * 0.8) - pixr;
	double ymarg = max(canh - histw, 0) / (2.0 * canh);
	double xmarg = 0.1 + ((double)pixr / (double)canw);

	TCanvas *mycan = new TCanvas("mycan","",canw,canh);
	mycan->SetMargin(0.1,xmarg,ymarg,ymarg); //left, right, bottom, top
	mycan->Draw();

	TH2I *XY = (TH2I*)infile->Get("FiberHitMap/Fiber_xy");
	XY->Draw("colz");

	TLine *myline = new TLine();
	myline->DrawLine(16.,16.,-16.,-16.);
	myline->DrawLine(-16.,16.,16.,-16.);

	TEllipse *mycir = new TEllipse(0,0,12.5);
	mycir->Draw();
	mycir->SetFillStyle(0);
}

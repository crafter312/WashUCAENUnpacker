void Plot_XY(int run = 0) {
	//gROOT->SetStyle("Plain");
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

	TCanvas *mycan = new TCanvas("mycan","",800,800);
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

{
	std::unique_ptr<TFile> myFile(TFile::Open("plot.root", "READ"));
	TBrowser b;

	// Style options
	gStyle->SetPalette(kBird);
	gStyle->SetOptStat(0);
	gPad->SetTickx();
	gPad->SetTicky();
}

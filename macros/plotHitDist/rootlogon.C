{
	gStyle->SetPalette(kBird);
	std::unique_ptr<TFile> myFile(TFile::Open("plot.root", "READ"));
	TBrowser b;

	gPad->SetTickx();
	gPad->SetTicky();
}

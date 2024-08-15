{
	gStyle->SetPalette(kBird);
	std::unique_ptr<TFile> myFile(TFile::Open("sort.root", "UPDATE"));
	TBrowser b;

	gPad->SetTickx();
	gPad->SetTicky();

	gROOT->ProcessLine(".L macros/HelperFunctions.C");
	gROOT->ProcessLine(".L macros/Plots.C");
}

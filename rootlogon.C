{
	gStyle->SetPalette(1);
	cout<<"Setting Palette"<<endl;
	std::unique_ptr<TFile> myFile(TFile::Open("sort.root", "UPDATE"));
	TBrowser b;

	gROOT->ProcessLine(".L macros/HelperFunctions.C");
	gROOT->ProcessLine(".L macros/CountsPerNumPhotons.C");
	gROOT->ProcessLine(".L macros/CompoundHistograms.C");
}

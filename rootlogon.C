{
	gStyle->SetPalette(1);
	cout<<"Setting Palette"<<endl;
	std::unique_ptr<TFile> myFile(TFile::Open("sort.root", "UPDATE"));
	TBrowser b;

	gPad->SetTickx();
	gPad->SetTicky();

	gSystem->AddIncludePath("$HOME/Desktop/SFA/caenUnpacker/include");

	gROOT->ProcessLine(".L macros/HelperFunctions.C");
	//gROOT->ProcessLine(".L macros/CountsPerNumPhotons.C");
	//gROOT->ProcessLine(".L macros/CompoundHistograms.C");
	gROOT->ProcessLine(".L macros/Plots.C");
}

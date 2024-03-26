//******** useful helper functions ********//

// finds the currently displayed histogram 
// (assumed to be the first TH1 in the primitives list)
const char* GetCurrentHistName() {
	TList* primitives = gPad->GetListOfPrimitives();
	for (TObject* obj: *primitives) {
		if (TH1* v = dynamic_cast<TH1*>(obj)) {
			return obj->GetName();
		}
	}
	return "";
}

// takes in the name of the relevant ROOT histogram
void SaveHist() {
	gDirectory->Get(GetCurrentHistName())->Write();
}

// takes in the run number of the run being saved
void SaveRun(const int& runnum) {
	string name = "run_" + std::to_string(runnum) + ".root";
	if (access(name.c_str(), F_OK) == -1) {
		std::ifstream  src("sort.root", std::ios::binary);
		std::ofstream  dst(name, std::ios::binary);
		dst << src.rdbuf();
	}
	else {
		int i = 0;
		name = "run_" + std::to_string(runnum) + "_";
		while (access((name + std::to_string(i) + ".root").c_str(), F_OK) != -1) {
			i++;
		}
		name = name + std::to_string(i) + ".root";
		std::ifstream  src("sort.root", std::ios::binary);
		std::ofstream  dst(name, std::ios::binary);
		dst << src.rdbuf();
	}
}

void SetAxisLabels(TH1* hist, const char* xlabel, const char* ylabel) {
	TAxis* axis = hist->GetXaxis();
	axis->SetTitle(xlabel);
	axis->CenterTitle();
	axis->SetTitleOffset(0.7);
	axis->SetTitleSize(0.06);
	axis = hist->GetYaxis();
	axis->SetTitle(ylabel);
	axis->CenterTitle();
	axis->SetTitleOffset(0.8);
	axis->SetTitleSize(0.06);
	hist->Draw("colz");
}
void SetAxisLabels(const char* xlabel, const char* ylabel) {
	TH1* hist = (TH1*)gDirectory->Get(GetCurrentHistName());
	TAxis* axis = hist->GetXaxis();
	axis->SetTitle(xlabel);
	axis->CenterTitle();
	axis->SetTitleOffset(0.7);
	axis->SetTitleSize(0.06);
	axis = hist->GetYaxis();
	axis->SetTitle(ylabel);
	axis->CenterTitle();
	axis->SetTitleOffset(0.8);
	axis->SetTitleSize(0.06);
	hist->Draw("colz");
}

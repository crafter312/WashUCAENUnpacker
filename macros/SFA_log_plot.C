{
	gROOT->Reset();

	// Set default style attributes
	TStyle * Sty = new TStyle("MyStyle","MyStyle");
	Sty->SetOptTitle(0);
	Sty->SetOptStat(0);
	Sty->SetLineWidth(4);
	Sty->SetPalette(55);
	Sty->SetCanvasColor(10);
	Sty->SetCanvasBorderMode(0);
	Sty->SetFrameLineWidth(0);
	Sty->SetFrameFillColor(10);
	Sty->SetPadColor(10);
	Sty->SetPadTickX(1);
	Sty->SetPadTickY(1);
	Sty->SetPadBottomMargin(.15);
	Sty->SetPadTopMargin(.03);
	Sty->SetPadLeftMargin(.15);
	Sty->SetPadRightMargin(.15);
	Sty->SetHistLineWidth(3);
	Sty->SetFuncWidth(3);
	Sty->SetFuncColor(kGreen);
	Sty->SetLineWidth(3);
	Sty->SetLabelSize(0.05,"xyz");
	Sty->SetLabelOffset(0.015,"y");
	Sty->SetLabelOffset(0.02,"x");
	Sty->SetLabelColor(kBlack,"xyz");
	Sty->SetTitleSize(0.06,"y");
	Sty->SetTitleSize(0.07,"x");
	Sty->SetTitleOffset(0.95,"y");
	Sty->SetTitleOffset(1.2,"x");
	Sty->SetTitleFillColor(10);
	Sty->SetTitleTextColor(kBlack);
	Sty->SetTickLength(.05,"xz");
	Sty->SetTickLength(.025,"y");
	Sty->SetNdivisions(10,"y");
	Sty->SetNdivisions(10,"x");
	Sty->SetEndErrorSize(0);
	Sty->SetTextFont(42);
	gROOT->SetStyle("MyStyle");
	gROOT->ForceStyle();

	// Commonly modified style attributes
	Sty->SetNdivisions(10,"xy");  // # tick mark divisions on the x and y axes
	Sty->SetTitleOffset(0.8,"y"); // y axis title offset
	Sty->SetTitleSize(0.06,"y");  // y axis title size
	Sty->SetTitleOffset(1.1,"x"); // x axis title offset
	Sty->SetTitleSize(0.06,"x");  // x axis title size

	double rightMargin = .03;
	Sty->SetPadBottomMargin(.15);
	Sty->SetPadTopMargin(.03);
	Sty->SetPadLeftMargin(.15);
	Sty->SetPadRightMargin(rightMargin);

	/******** GRAPH SETUP ********/

	TCanvas c1("c1","",1024,576);

	vector<double> x = { 2.8643, 11.6974, 126.2623 };
	vector<double> y1 = { 112.7738714, 167.3561754, 196.5275078 }; // Blue fiber #39
	vector<double> ey1 = { 8.378893008, 5.655347947, 4.842974322 };
	vector<double> y2 = { 64.87892617, 119.7162428, 162.3584308 }; // Blue fiber #30
	vector<double> ey2 = { 7.557884182, 5.375970406, 9.616910091 };
	vector<double> y3 = { 46.83802282, 80.03656414, 126.9998401 }; // Blue fiber #21
	vector<double> ey3 = { 7.186193695, 9.889283142, 3.711568121 };

	// Get minimum axis value from third graph
	TGraphErrors frame3(3, x.data(), y3.data(), nullptr, ey3.data());
	frame3.SetMarkerStyle(20);	
	frame3.SetMarkerSize(1.5);
	frame3.Draw();
	double xmin = frame3.GetYaxis()->GetXmin();

	c1.Clear();

	// Make graphs (for real this time)
	TGraphErrors frame1(3, x.data(), y1.data(), nullptr, ey1.data());
	frame1.GetXaxis()->SetTitle("LISE++ #DeltaE (MeV)");
	frame1.GetXaxis()->CenterTitle();
	frame1.GetYaxis()->SetTitle("ToT (arbitrary units)");
	frame1.GetYaxis()->CenterTitle();
	frame1.SetMarkerStyle(20);
	frame1.SetMarkerSize(1.5);
	frame1.Draw("ap");

	frame1.GetYaxis()->SetRangeUser(xmin, frame1.GetYaxis()->GetXmax());

	TGraphErrors frame2(3, x.data(), y2.data(), nullptr, ey2.data());
	frame2.SetMarkerStyle(20);	
	frame2.SetMarkerSize(1.5);
	frame2.SetMarkerColor(kRed);
	frame2.SetLineColor(kRed);
	frame2.Draw("samep");
	
	frame3.SetMarkerColor(kBlue);
	frame3.SetLineColor(kBlue);
	frame3.Draw("samep");

	// Apply log fits
	TF1 fit1("fit1", "[0]*TMath::Log([1]*x)", 0, 200);
	fit1.SetParameter(0, 30);
	fit1.SetParameter(1, 300);
	fit1.SetParLimits(0, 0, 100);
	fit1.SetParLimits(1, 0, 1000);
	fit1.SetLineColor(kBlack);
	fit1.SetLineStyle(7);
	frame1.Fit(&fit1);

	TF1 fit2("fit2", "[0]*TMath::Log([1]*x)", 0, 200);
	fit2.SetParameter(0, 20);
	fit2.SetParameter(1, 100);
	fit2.SetParLimits(0, 0, 100);
	fit2.SetParLimits(1, 0, 300);
	fit2.SetLineColor(kRed);
	fit2.SetLineStyle(7);
	frame2.Fit(&fit2);

	TF1 fit3("fit3", "[0]*TMath::Log([1]*x)", 0, 200);
	fit3.SetParameter(0, 10);
	fit3.SetParameter(1, 100);
	fit3.SetParLimits(0, 0, 100);
	fit3.SetParLimits(1, 0, 300);
	fit3.SetLineColor(kBlue);
	fit3.SetLineStyle(7);
	frame3.Fit(&fit3);

	// Draw legend
	TLegend* legend = new TLegend(0.7, 0.25, 0.9, 0.4);
	legend->AddEntry(&frame1, "Blue fiber #39", "L");
	legend->AddEntry(&frame2, "Blue fiber #30", "L");
	legend->AddEntry(&frame3, "Blue fiber #21", "L");
	legend->Draw();

	// Save figure
	c1.Print("SFA_log_plot_exp.png", "png");
	c1.Print("SFA_log_plot_exp.eps", "eps");
}

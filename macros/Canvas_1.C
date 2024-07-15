#ifdef __CLING__
#pragma cling optimize(0)
#endif
void Canvas_1()
{
//=========Macro generated from canvas: Canvas_1/Canvas_1
//=========  (Thu Jul 11 15:00:01 2024) by ROOT version 6.26/10
   TCanvas *Canvas_1 = new TCanvas("Canvas_1", "Canvas_1",308,161,363,329);
   Canvas_1->Range(-8.000001,-15.88125,72,142.9313);
   Canvas_1->SetFillColor(0);
   Canvas_1->SetBorderMode(0);
   Canvas_1->SetBorderSize(2);
   Canvas_1->SetTickx(1);
   Canvas_1->SetTicky(1);
   Canvas_1->SetFrameBorderMode(0);
   Canvas_1->SetFrameBorderMode(0);
   
   TH1F *Fiber_postotx__1 = new TH1F("Fiber_postotx__1","Blue Fiber (Horizontal) Example Event (21 MeV/u a)",64,0,64);
   Fiber_postotx__1->SetBinContent(19,23);
   Fiber_postotx__1->SetBinContent(20,121);
   Fiber_postotx__1->SetBinContent(21,30);
   
   TPaveStats *ptstats = new TPaveStats(0.9253731,0.9327217,0.9962687,0.9938838,"brNDC");
   ptstats->SetName("stats");
   ptstats->SetBorderSize(1);
   ptstats->SetFillColor(0);
   ptstats->SetLineColor(0);
   ptstats->SetTextAlign(12);
   ptstats->SetTextColor(0);
   ptstats->SetTextFont(42);
   TText *ptstats_LaTex = ptstats->AddText("Fiber_postotx");
   ptstats_LaTex->SetTextSize(0.01406728);
   ptstats_LaTex = ptstats->AddText("Entries = 0      ");
   ptstats_LaTex = ptstats->AddText("Mean  =      0");
   ptstats_LaTex = ptstats->AddText("Std Dev   =      0");
   ptstats->SetOptStat(1111);
   ptstats->SetOptFit(0);
   ptstats->Draw();
   Fiber_postotx__1->GetListOfFunctions()->Add(ptstats);
   ptstats->SetParent(Fiber_postotx__1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   Fiber_postotx__1->SetLineColor(ci);
   Fiber_postotx__1->GetXaxis()->SetTitle("Fiber #");
   Fiber_postotx__1->GetXaxis()->CenterTitle(true);
   Fiber_postotx__1->GetXaxis()->SetLabelFont(42);
   Fiber_postotx__1->GetXaxis()->SetTitleSize(0.06);
   Fiber_postotx__1->GetXaxis()->SetTitleOffset(0.66);
   Fiber_postotx__1->GetXaxis()->SetTitleFont(42);
   Fiber_postotx__1->GetYaxis()->SetTitle("ToT (channels)");
   Fiber_postotx__1->GetYaxis()->CenterTitle(true);
   Fiber_postotx__1->GetYaxis()->SetLabelFont(42);
   Fiber_postotx__1->GetYaxis()->SetTitleSize(0.06);
   Fiber_postotx__1->GetYaxis()->SetTitleOffset(0.6);
   Fiber_postotx__1->GetYaxis()->SetTitleFont(42);
   Fiber_postotx__1->GetZaxis()->SetLabelFont(42);
   Fiber_postotx__1->GetZaxis()->SetTitleOffset(1);
   Fiber_postotx__1->GetZaxis()->SetTitleFont(42);
   Fiber_postotx__1->Draw("");
   
   TPaveText *pt = new TPaveText(0.1998881,0.9326147,0.8998881,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *pt_LaTex = pt->AddText("Blue Fiber (Horizontal) Example Event (21 MeV/u a)");
   pt->Draw();
   Canvas_1->Modified();
   Canvas_1->cd();
   Canvas_1->SetSelected(Canvas_1);
}

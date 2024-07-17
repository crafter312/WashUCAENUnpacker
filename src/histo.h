#ifndef histo_
#define histo_
// class used to store ROOT histograms. When defining a new histogram make sure to 
// define it in both the header and here.
// created 6/9/2021 by Nicolas Dronchi

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include "TH1F.h"
#include "TH1I.h"
#include "TH2I.h"
#include "TFile.h"
#include "TGraph.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TTree.h"

#include "CAENd5202.h"
#include "fiber.h"

using namespace std;

class histo
{
protected:
  TFile* file_read; // output root file

	// Single events
  TTree* t;
	Event event;

	// Matched events
	TTree* tmatch;
	fiber fib;
	Event red;
	Event blue;

public:
  histo();
  ~histo();
	void InitSpecMode();
	void FillTree(Event);
	void FillMatchedTree(fiber*, Event*, Event*);
  void clear();

	Event* GetRedEvent() { return &red; }
	Event* GetBlueEvent() { return &blue; }

	TH1I* toa_hist;
	TH2I* tot_summary_blue;
	TH2I* tot_summary_red;

  TDirectoryFile* dirHitMap; // directory for plotting xy-correlated event parameters

	TH2I* Fiber_ixiy;
	TH2I* Fiber_xy;
	TH2I* Fiber_tot_summary_x;
	TH2I* Fiber_tot_summary_y;
  TH1F* Fiber_postotx;
  TH1F* Fiber_postoty;
  TH1F* Fiber_postoax;
  TH1F* Fiber_postoay;
  TH1I* Fiber_toax;
  TH1I* Fiber_toay;
};
#endif

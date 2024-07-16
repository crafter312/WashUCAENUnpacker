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
  TFile* file_read; //!< output root file

  TTree* t;
	double tstamp;
  unsigned char boardID;
  vector<float> tot;
  vector<float> toa;
	vector<unsigned char> chan;
  vector<int> pos;

	// Matched events
	TTree* tmatch;
	fiber fib;
	Event red;
	Event blue;

public:
  histo();  //!< constructor
  ~histo();
	void InitSpecMode();
	void FillTree(double, unsigned char, vector<eventTiming>);
	void FillMatchedTree(fiber*, Event*, Event*);
  void clear();

  TDirectoryFile* dirHitMap; //!< directory for plotting xy fiber correlations

  TH1F* tot_hist;
  TH1F* toa_hist;

	TH2I* Fiber_ixiy;
	TH2I* Fiber_xy;
  TH1F* Fiber_postotx;
  TH1F* Fiber_postoty;
  TH1F* Fiber_postoax;
  TH1F* Fiber_postoay;
  TH1I* Fiber_toax;
  TH1I* Fiber_toay;
};
#endif

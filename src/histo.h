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

using namespace std;

class histo
{
protected:
  TFile* file_read; //!< output root file

  TTree* t;
	double tstamp;
  unsigned short low;
	unsigned short high;
  float tot;
  float toa;

public:
  histo();  //!< constructor
  ~histo();
	void InitSpecMode();
  void FillTree(double, unsigned short, unsigned short, float, float);
	void FillTree(double, float, float);

  TH1I* lg_hist;
  TH1F* tot_hist;
  TH1F* toa_hist;
  TH2F* tot_lg_hist;
};
#endif

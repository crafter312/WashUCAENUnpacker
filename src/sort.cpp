//File created to read CAEN D5202 output data
//created by Charlie Fallon 12/01/2023

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "det.h"
#include "histo.h"
#include "CAENd5202.h"
#include <ctime>

using namespace std;

int main(int argc, char* argv[])
{
  // get run # from command line arguments
  if (argc == 1) throw invalid_argument("must specify at least one run #");
  string runnum = argv[1];
  stoi(runnum);

  // start clock
  clock_t t;
  t = clock();

  //*********************************************************************************************** 
  
  // to analyze mulitple files at once:
  // IMPORTANT: make sure you add all the "namein"'s you're using :)
  
  string namein1 = "/home/Li6Webb/Desktop/SFA/ScintFiberData/Run" + runnum + "_list.dat";

  vector<string> files;
  files.push_back(namein1);
  string namein;
  
  histo * Histo = new histo(); // histo class stores all the histograms created
	double distance = 100.;      // distance in mm from target to detector
  det Det(Histo, distance);    // det class is where we store all of the events and analyse them
  
  for (int i = 0; i < files.size(); i++)
  {
    namein = files[i];
    cout << "reading file: " << namein << endl;
  
    // open binary data file
    ifstream evtfile;
    evtfile.open(namein.c_str(), ios::binary);

    // checking to see if we can open the file correctly
    if (evtfile.bad() ) cout << "bad " << evtfile.bad() << endl;
    if (evtfile.fail() ) cout << "fail " << evtfile.bad() << endl;
    if (!evtfile)
    {
      cout << "could not open event file" << endl;
      abort();
    }
    
    // unpacks all events and saves them in vectors for pulses and temperature events
		// event matching and analysis is done here as well
    Det.unpack(&evtfile);
    // we are done with the data file at this point
    evtfile.close();
  }
  
  cout << "total (unmatched) singles: " << Det.Nsingles << " " << (float)Det.Nsingles/(float)(Det.Nsingles+Det.Nmatched) * 100 << "\%"<< endl;
  cout << "total (matched) events: " << Det.Nmatched << " " << (float)Det.Nmatched/(float)(Det.Nsingles+Det.Nmatched) * 100 << "\%" << endl;
	cout << "good single blue events: " << Det.goodBlue << endl;
	cout << "good single red events: " << Det.goodRed << endl;
	cout << "good matched events: " << Det.goodMatched << endl;
	cout << "red (back) efficiency: " << (double)Det.goodMatched / (double)(Det.goodMatched + Det.goodRed) * 100. << "\%" << endl;
	cout << "blue (front) efficiency: " << (double)Det.goodMatched / (double)(Det.goodMatched + Det.goodBlue) * 100. << "\%" << endl;

  delete Histo; //need to delete the Histo because that is where the root files are written out
  
  t = clock()-t;
  cout << "calculation time: " << (float) t/CLOCKS_PER_SEC/60 << " min" << endl;

  return 0;
}

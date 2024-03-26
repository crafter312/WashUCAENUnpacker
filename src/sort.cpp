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
  
  string namein1 = "/home/Li6Webb/Desktop/caenUnpacker/DAQ/Run" + runnum + "_list.dat";

  vector<string> files;
  files.push_back(namein1);
  string namein;
  
  histo * Histo = new histo();  // histo class stores all the histograms created
  det Det(Histo);               // det class is where we store all of the events and analyse them
  
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
    Det.unpack(&evtfile);
    // we are done with the data file at this point
    evtfile.close();
  }
  

  

  //here all the analysis is done on the vector of events
  //Det.analyze();
  
  //cout << "total number of pulses: " << Det.temperatures.size() << endl; //This isn't really the
                                                                           //total # pulses tho :/
  
  delete Histo; //need to delete the Histo because that is where the root files are written out
  
  t = clock()-t;
  cout << "calculation time: " << (float) t/CLOCKS_PER_SEC/60 << " min" << endl;

  return 0;
}

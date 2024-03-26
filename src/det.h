#ifndef det_
#define det_
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "histo.h"
#include "CAENd5202.h"

using namespace std;

class det
{
 public:
  det(histo* Histo);
  ~det();
  histo* Histo;

  bool unpack(ifstream *);
  
  Event* SIPMevent;
  long nevts;
  
};
#endif

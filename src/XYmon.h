#ifndef XYmon_
#define XYmon_

#include "fiber.h"
#include <cmath>

class XYmon
{
 public:
  XYmon();
  ~XYmon();
  void setDistance(float);
  fiber * vert;
  fiber * horz;

  bool has_data;
  int ix,iy;
  float x,y,r,theta,phi;
  float thetadeg,phideg;
  float distance;
  
  void reset();
  bool make_2d();
};

#endif

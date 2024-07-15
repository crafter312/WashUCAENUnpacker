#include "XYmon.h"

//constructor
XYmon::XYmon()
{
  vert = new fiber();
  horz = new fiber();
}

XYmon::~XYmon()
{
  delete vert;
  delete horz;
}


void XYmon::setDistance(float dist)
{
  distance = dist;
}
void XYmon::reset()
{
  vert->reset();
  horz->reset();
  x = -999;
  y = -999;
  theta = -999;
  phi = -999;
  thetadeg = -999;
  phideg = -999;
  has_data = false;
}



bool XYmon::make_2d()
{

  vert->make_2d();
  horz->make_2d();
  has_data = vert->has_data && horz->has_data;

  if (!has_data) {return has_data;}

  //cout << "pmtx,pmty " << vert->pmtx << "," << vert->pmty << "    ix,iy ";
  //cout << vert->ix << "," << vert->iy << "    posID " << vert->posID << endl;
  
  //posID is 1->64
  x = -1*((float)horz->posID-0.5)*0.25 + 8; //mm
  y = -1*((float)vert->posID-0.5)*0.25 + 8; //mm

  r = sqrt(pow(x,2) + pow(y,2));
  r /= 10.;  //cm
  theta = atan(r/distance);
  phi = atan2(y,x);
  thetadeg = theta*180./acos(-1);
  phideg = phi*180./acos(-1);

  //cout << "x,y " << x << "," << y << "  r " << r << "  theta " << theta << "rad  " << thetadeg << "deg" << endl;

  return has_data;
}


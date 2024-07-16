#include "fiber.h"
#include <algorithm>

fiber::fiber(float dist)
{
  distance = dist;
}

void fiber::reset()
{
  ix = -1;
  iy = -1;

	x = -1;
	y = -1;

  has_data = false;
}

//horz fiber gives horizontal position, blue fiber (which runs accross 
//vert fiber gives vertical position, red fiber
bool fiber::make_2d(Event* horz, Event* vert) 
{
  //assign vert a position ID
	//start with finding max and sums
	posmaxhorz = 0; //max value positions in dataTiming which is type vector<eventTiming>
	posmaxvert = 0;

	sumhorz = 0; //sum of the Tot-thresh values 
	sumvert = 0;
  float momhorz = 0;
  float momvert = 0;

	int threshhorz = 0; //thresholds to be subtracted off of ToT values
	int threshvert = 0;

	multhorz = horz->GetNHits();
	multvert = vert->GetNHits();

  for (int i = 0; i < horz->GetNHits(); i++) {
		int PHraw = horz->GetTimingEvent(i).ToTmatched;
		int PH = max(PHraw - threshhorz, 0);
		sumhorz += PH;
    momhorz += PH * horz->GetTimingEvent(i).pos;

		if (PHraw > horz->GetTimingEvent(posmaxhorz).ToTmatched)
			posmaxhorz = i;
  }
  for (int i = 0; i < vert->GetNHits(); i++) {
		int PHraw = vert->GetTimingEvent(i).ToTmatched;
		int PH = max(PHraw - threshvert, 0);
		sumvert += PH;
    momvert += PH * vert->GetTimingEvent(i).pos;

		if (PHraw > vert->GetTimingEvent(posmaxvert).ToTmatched)
			posmaxvert = i;
  }

	ix = horz->GetTimingEvent(posmaxhorz).pos;
	iy = vert->GetTimingEvent(posmaxvert).pos;


	// CHANNEL TO FIBER
	// fib = ((iCh - (iCh % 2)) / 2) + ((iCh % 2) * 32)

  // Calculate center of gravity
  float fibhorz = momhorz / sumhorz;
  float fibvert = momvert / sumvert;

  //posID is 1->64
  //x = -1*((float)ix-0.5)*0.5 + 16; //mm
  //y = -1*((float)iy-0.5)*0.5 + 16; //mm
  x = -1*(fibhorz-0.5)*0.5 + 16; //mm
  y = -1*(fibvert-0.5)*0.5 + 16; //mm

  r = sqrt(pow(x,2) + pow(y,2));
  r /= 10.;  //cm
  theta = atan(r/distance);
  phi = atan2(y,x);
  thetadeg = theta*180./acos(-1);
  phideg = phi*180./acos(-1);

  has_data = true;

  return has_data;
}

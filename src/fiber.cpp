#include "fiber.h"
#include <algorithm>

fiber::fiber() {}

/**
 * This function takes in a pair of matched fiber events and performs a center of gravity
 * calculation (first statistical moment), along with calculating other values for the
 * given event pair. Note also that the fiber layers are as such:
 * 
 * Horzontal (blue) fiber gives x position
 * Vertical (red) fiber gives y position
 */
bool fiber::make_2d(Event* horz, Event* vert, float distance) {
	// Set default values	
	posmaxhorz = 0;
	posmaxvert = 0;
	sumhorz = 0;
	sumvert = 0;

	// Advance declaration of variables
  float momhorz = 0;
  float momvert = 0;
	int threshhorz = 0;
	int threshvert = 0;

	int mult = horz->GetNHits();
  for (int i = 0; i < mult; i++) {
		int PHraw = horz->GetTimingEvent(i).ToTmatched;
		int PH = max(PHraw - threshhorz, 0);
		sumhorz += PH;
    momhorz += PH * horz->GetTimingEvent(i).pos;

		if (PHraw > horz->GetTimingEvent(posmaxhorz).ToTmatched)
			posmaxhorz = i;
  }
	mult = vert->GetNHits();
  for (int i = 0; i < mult; i++) {
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

  return true;
}

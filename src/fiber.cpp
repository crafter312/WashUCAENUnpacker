// Created by Nicholas Dronchi
// Modified by Henry Webb 

#include "fiber.h"

#include "CAENd5202.h"
#include "constants.h"

#include <algorithm>
#include <iostream>
#include <math.h>

fiber::fiber() {}

/**
 * This function takes in a pair of matched fiber events and performs a center of gravity
 * calculation (first statistical moment), along with calculating other values for the
 * given event pair. Note also that the fiber layers are as such:
 * 
 * Horzontal (blue) fiber gives x position
 * Vertical (red) fiber gives y position
 */
bool fiber::make_2d(Event* horz, Event* vert, double distance) {
	// Set default values	
	posmaxhorz = 0;
	posmaxvert = 0;
	sumhorz = 0;
	sumvert = 0;

	// Advance declaration of variables
  double momhorz = 0;
  double momvert = 0;
	double threshhorz = 0;
	double threshvert = 0;

	// Horizontal fibers
	double maxToT = horz->FindMax([](eventTiming a, eventTiming b) -> bool { a.ToTmatched < b.ToTmatched; }).ToTmatched;
	int mult = horz->GetNHits();
  for (int i = 0; i < mult; i++) {
		double PHraw = horz->GetTimingEvent(i).ToTmatched;
		double PH = max(PHraw - threshhorz, 0.);
		sumhorz += PH;
    momhorz += PH * ((double)horz->GetTimingEvent(i).pos);

		if (PHraw > horz->GetTimingEvent(posmaxhorz).ToTmatched)
			posmaxhorz = i;
  }
	mult = vert->GetNHits();
  for (int i = 0; i < mult; i++) {
		double PHraw = vert->GetTimingEvent(i).ToTmatched;
		double PH = max(PHraw - threshvert, 0.);
		sumvert += PH;
    momvert += PH * ((double)vert->GetTimingEvent(i).pos);

		if (PHraw > vert->GetTimingEvent(posmaxvert).ToTmatched)
			posmaxvert = i;
  }

	ix = horz->GetTimingEvent(posmaxhorz).pos;
	iy = vert->GetTimingEvent(posmaxvert).pos;


	// CHANNEL TO FIBER
	// fib = ((iCh - (iCh % 2)) / 2) + ((iCh % 2) * 32)

  // Calculate center of gravity
  double fibhorz = momhorz / sumhorz;
  double fibvert = momvert / sumvert;

  //posID is 1->64
  //x = -1*((float)ix-0.5)*0.5 + 16; //mm
  //y = -1*((float)iy-0.5)*0.5 + 16; //mm
  x = -1*(fibhorz-0.5)*0.5 + 16; //mm
  y = -1*(fibvert-0.5)*0.5 + 16; //mm

  r = sqrt((x * x) + (y * y));
  r /= 10.;  //cm
  double theta = atan(r / distance);
  double phi = atan2(y , x);
  thetadeg = theta * rad_to_deg;
  phideg = phi * rad_to_deg;

  return true;
}

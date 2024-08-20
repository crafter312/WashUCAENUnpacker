// Created by Nicholas Dronchi
// Modified by Henry Webb 

#include "fiber.h"

#include "CAENd5202.h"
#include "constants.h"

#include <algorithm>
#include <iostream>
#include <math.h>

fiber::fiber() {}

void fiber::clear() {
	posmaxhorz = 0;
	posmaxvert = 0;
	posmaxhorznew = -1;
	posmaxvertnew = 0;
	sumhorz = 0;
	sumvert = 0;
	multTrimmedX = 0;
	multTrimmedY = 0;
	tstampdiff = 0;
	tdiffx.clear();
	tdiffy.clear();
}

/**
 * This function takes in a pair of matched fiber events and performs a center of gravity
 * calculation (first statistical moment), along with calculating other values for the
 * given event pair. The time gate values applied on a hit-wise basis were determined by
 * looking at histograms for the time difference of all the hits in both layers with the
 * "center hit" in the front layer (i.e. the front layer hit with the largest ToT). Note
 * also that the fiber layers are as such:
 * 
 * Horzontal (blue) fiber gives x position
 * Vertical (red) fiber gives y position
 */
bool fiber::make_2d(Event* horz, Event* vert, double distance) {
	// Set default values
	clear();

	// Apply front layer ToA gate, find center hit
	eventTiming ev;
	int mult = horz->GetNHits();
	double maxToT = 0;
	for (int i = 0; i < mult; i++) {
		ev = horz->GetTimingEvent(i);
		if ((ev.ToA < 650) || (ev.ToA > 740) || (ev.ToTmatched < maxToT)) continue;

		maxToT = ev.ToTmatched;
		posmaxhorznew = i;
	}
	if (posmaxhorznew == -1) return false;

	// Lambda function for max hit finding
	auto comp = [](eventTiming a, eventTiming b) -> bool { return a.ToTmatched < b.ToTmatched; };

	// Advance declaration of variables
  double momhorz = 0;
  double momvert = 0;
	double threshhorz = 0;
	double threshvert = 0;
	double PH;
	int tdiff;

	// Horizontal fibers (front layer)
	eventTiming maxHitBlue = horz->GetTimingEvent(posmaxhorznew);
	ix = maxHitBlue.pos;
  for (int i = 0; i < mult; i++) {

		// Get index of max ToT hit
		ev = horz->GetTimingEvent(i);
		if (ev.ToTmatched > horz->GetTimingEvent(posmaxhorz).ToTmatched)
			posmaxhorz = i;

		// Apply hit-wise time gate
		tdiff = ev.ToA - maxHitBlue.ToA;
		tdiffx.push_back(tdiff);
		if (tdiff < 0 || tdiff > 30) continue;

		// Calculate values
		PH = max(ev.ToTmatched - threshhorz, 0.);
		sumhorz += PH;
    momhorz += PH * ((double)ev.pos);
		multTrimmedX++;
  }

	// Vertical fibers (back layer)
	tstampdiff = vert->GetTimeStamp() - horz->GetTimeStamp();
	eventTiming maxHitRed = vert->FindMax(comp);
	iy = maxHitRed.pos;
	mult = vert->GetNHits();
  for (int i = 0; i < mult; i++) {

		// Get index of max ToT hit
		ev = vert->GetTimingEvent(i);
		if (ev.ToTmatched == maxHitRed.ToTmatched)
			posmaxvertnew = i;
		if (ev.ToTmatched > vert->GetTimingEvent(posmaxvert).ToTmatched)
			posmaxvert = i;

		// Apply hit-wise time gate
		tdiff = ev.ToA - maxHitBlue.ToA + tstampdiff;
		tdiffy.push_back(tdiff);
		if (tdiff < 500 || tdiff > 540) continue;
		
		// Calculate values
		PH = max(ev.ToTmatched - threshvert, 0.);
		sumvert += PH;
    momvert += PH * ((double)ev.pos);
		multTrimmedY++;
  }

	// CHANNEL TO FIBER
	// fib = ((iCh - (iCh % 2)) / 2) + ((iCh % 2) * 32)

  // Calculate center of gravity
  double fibhorz = momhorz / sumhorz;
  double fibvert = momvert / sumvert;

  // Calculate physical parameters of matched event
  x = -1*(fibhorz-0.5)*0.5 + 16;              //mm
  y = -1*(fibvert-0.5)*0.5 + 16;              //mm
  r = sqrt((x * x) + (y * y)) * 0.1;          //cm
  thetadeg = atan(r / distance) * rad_to_deg; //deg
  phideg = atan2(y , x) * rad_to_deg;         //deg

	// Calculate additional values
	xdev = x - (-1*(maxHitBlue.pos-0.5)*0.5 + 16);
	ydev = y - (-1*(maxHitRed.pos-0.5)*0.5 + 16);

  return true;
}

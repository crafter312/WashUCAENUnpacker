// Created by Nicholas Dronchi
// Modified by Henry Webb
//                 _.-.
//          _.----'    `.
//         /             `.
//        /                `-._
// VK   .'     __.----.        `-._
//   .-'      /        `-._       =
//  //       /             `-.    =
//   //    .'                 `--.=
//    // .'
//     //

#include "fiber.h"

#include "CAENd5202.h"
#include "constants.h"

#include <algorithm>
#include <iostream>
#include <cmath>

using namespace std;

fiber::fiber() {}

void fiber::clear() {
	posmaxhorz = -1;
	posmaxvert = -1;
	sumhorz = 0;
	sumvert = 0;
	multTrimmedX = 0;
	multTrimmedY = 0;
	tstampdiff = 0;
	tdiffx.clear();
	tdiffy.clear();
	badtx = false;
	badty = false;
	xindices.clear();
	yindices.clear();
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
tuple<bool, bool> fiber::make_2d(Event& horz, Event& vert, double distance) {
	// Set default values
	clear();

	// Apply front layer ToA gate, find center hit
	eventTiming ev;
	int mult = horz.GetNHits();
	double maxToT = 0;
	double maxToTalt1 = 0;
	int posmaxhorzalt = 0;
	for (int i = 0; i < mult; i++) {

		// Find alternate max ToT hit index
		ev = horz.GetTimingEvent(i);
		if (ev.ToTmatched > maxToTalt1) {
			maxToTalt1 = ev.ToTmatched;
			posmaxhorzalt = i;
		}
		
		if ((ev.ToA < 650) || (ev.ToA > 740) || (ev.ToTmatched < maxToT)) continue;

		maxToT = ev.ToTmatched;
		posmaxhorz = i;
	}

	if (posmaxhorz == -1) {
		posmaxhorz = posmaxhorzalt;
		badtx = true;
	}

	// Advance declaration of variables
  double momhorz = 0;
  double momvert = 0;
	double threshhorz = 0;
	double threshvert = 0;
	double sumfibhorz = 0;
	double sumfibvert = 0;
	double PH;
	double tdiff;
	int tempi;

	// Horizontal fibers (front layer)
	eventTiming maxHitBlue = horz.GetTimingEvent(posmaxhorz);
	ix = maxHitBlue.pos;
  for (int i = 0; i < mult; i++) {

		// Apply hit-wise time gate
		ev = horz.GetTimingEvent(i);
		tdiff = (double)(ev.ToA - maxHitBlue.ToA) * 0.5; //ns
		tdiffx.push_back(tdiff);
		if ((tdiff < 0.) || (tdiff > 15.)) continue;

		// Calculate values
		PH = max(ev.ToTmatched - threshhorz, 0.);
		sumhorz += PH;
    momhorz += PH * ev.pos;
		sumfibhorz += ev.pos;
		multTrimmedX++;
		tempi = i;
		xindices.push_back(tempi);
  }

	// Vertical fibers (back layer)
	tstampdiff = (vert.GetTimeStamp() - horz.GetTimeStamp()) * 1000.; //ns
	mult = vert.GetNHits();
	maxToT = 0;
	double maxToTalt2 = 0;
	int posmaxvertalt;
  for (int i = 0; i < mult; i++) {

		// Find alternate max ToT hit index
		ev = vert.GetTimingEvent(i);
		if (ev.ToTmatched > maxToTalt2) {
			maxToTalt2 = ev.ToTmatched;
			posmaxvertalt = i;
		}

		// Apply hit-wise time gate
		//tdiff = (double)(ev.ToA - maxHitBlue.ToA) * 0.5 + tstampdiff; //ns
		tdiff = (double)(ev.ToA - maxHitBlue.ToA) * 0.5; //ns
		tdiffy.push_back(tdiff);
		//if (tdiff < 230. || tdiff > 270.) continue;
		if((tdiff < -40.) || (tdiff > 32.)) continue;

		// Get index of max ToT hit
		if (ev.ToTmatched > maxToT) {
			maxToT = ev.ToTmatched;
			posmaxvert = i;
		}

		// Calculate values
		PH = max(ev.ToTmatched - threshvert, 0.);
		sumvert += PH;
    momvert += PH * ev.pos;
		sumfibvert += ev.pos;
		multTrimmedY++;
		tempi = i;
		yindices.push_back(tempi);
  }

	// For testing, to analyze events that don't pass red fiber hit time gate
	if (posmaxvert == -1) {
		posmaxvert = posmaxvertalt;
		badty = true;
	}

	eventTiming maxHitRed = vert.GetTimingEvent(posmaxvert);
	iy = maxHitRed.pos;

	// CHANNEL TO FIBER
	// fib = ((iCh - (iCh % 2)) / 2) + ((iCh % 2) * 32)

  // Calculate center of gravity
  double fibhorz = momhorz / sumhorz;
  double fibvert = momvert / sumvert;
	avgfibx = sumfibhorz / multTrimmedX;
	avgfiby = sumfibvert / multTrimmedY;

  // Calculate physical parameters of matched event
  x = -1*(fibhorz-0.5)*0.5 + 16;              //mm
  y = -1*(fibvert-0.5)*0.5 + 16;              //mm
  r = sqrt((x * x) + (y * y)) * 0.1;          //cm
  thetadeg = atan(r / distance) * rad_to_deg; //deg
  phideg = atan2(y , x) * rad_to_deg;         //deg

	// Calculate additional values
	xdev = x - (-1*(ix-0.5)*0.5 + 16); //mm
	ydev = y - (-1*(iy-0.5)*0.5 + 16); //mm

	//return { !badtx && (maxHitBlue.ToTmatched > 30.) && (maxHitBlue.ToTmatched < 120.), !badty && (maxHitRed.ToTmatched > 25.) && (maxHitRed.ToTmatched < 70.) };   // carbon
  return { !badtx && (maxHitBlue.ToTmatched > 76.) && (maxHitBlue.ToTmatched < 150.), !badty && (maxHitRed.ToTmatched > 55.) && (maxHitRed.ToTmatched < 85.) };   // alphas
	//return { !badtx && (maxHitBlue.ToTmatched > 140.) && (maxHitBlue.ToTmatched < 200.), !badty && (maxHitRed.ToTmatched > 75.) && (maxHitRed.ToTmatched < 140.) }; // carbon
}

bool fiber::isGoodSingle(Event& single, double min, double max) {
	// Apply ToA gate, find center hit
	eventTiming ev;
	int mult = single.GetNHits();
	double maxToT = 0;
	int posmaxalt = -1;
	for (int i = 0; i < mult; i++) {
		ev = single.GetTimingEvent(i);
	
		//if ((ev.ToA < 660) || (ev.ToA > 735) || (ev.ToTmatched < maxToT)) continue; // deuterons
		if ((ev.ToA < 650) || (ev.ToA > 730) || (ev.ToTmatched < maxToT)) continue; // alphas
		//if ((ev.ToA < 650) || (ev.ToA > 740) || (ev.ToTmatched < maxToT)) continue; // carbon

		maxToT = ev.ToTmatched;
		posmaxalt = i;
	}
	if (posmaxalt == -1) return false;

	eventTiming maxHit = single.GetTimingEvent(posmaxalt);
	return (maxHit.ToTmatched > min) && (maxHit.ToTmatched < max);
}

bool fiber::isGoodSingleRed(Event& red) {
	//return isGoodSingle(red, 25, 70);  // deuteron
	return isGoodSingle(red, 55, 85);  // alphas
	//return isGoodSingle(red, 75, 140); // carbon
}

bool fiber::isGoodSingleBlue(Event& blue) {
	//return isGoodSingle(blue, 30, 120);  // deuterons
	return isGoodSingle(blue, 76, 150);  // alphas
	//return isGoodSingle(blue, 140, 200); // carbon
}

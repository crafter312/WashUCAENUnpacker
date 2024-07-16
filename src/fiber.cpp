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
	int threshhorz = 0; // ToT threshold to eliminate small amplitude noise hits
	int threshvert = 0;
	eventTiming ev;
	int PH;

	int mult = horz->GetNHits();
  for (int i = 0; i < mult; i++) {
		ev = horz->GetTimingEvent(i);
		PH = max(ev.ToTmatched - threshhorz, 0.);
		sumhorz += PH;
    momhorz += PH * ev.pos;

		if (ev.ToTmatched > horz->GetTimingEvent(posmaxhorz).ToTmatched)
			posmaxhorz = i;
  }
	mult = vert->GetNHits();
  for (int i = 0; i < mult; i++) {
		ev = vert->GetTimingEvent(i);
		PH = max(ev.ToTmatched - threshvert, 0.);
		sumvert += PH;
    momvert += PH * ev.pos;

		if (ev.ToTmatched > vert->GetTimingEvent(posmaxvert).ToTmatched)
			posmaxvert = i;
  }

	ix = horz->GetTimingEvent(posmaxhorz).pos;
	iy = vert->GetTimingEvent(posmaxvert).pos;

  // Calculate center of gravity (first statistical moment)
  x = -1 * ((momhorz / sumhorz) - 0.5) * 0.5 + 16; //mm
  y = -1 * ((momvert / sumvert) - 0.5) * 0.5 + 16; //mm

	// Other physical parameters
  r = sqrt(pow(x, 2) + pow(y, 2)) / 10.; //cm
  theta = atan(r / distance);
  phi = atan2(y, x);
  thetadeg = theta * 180. / acos(-1);
  phideg = phi * 180. / acos(-1);

  return true;
}

/*Published under The MIT License (MIT)

See LICENSE.TXT*/

// Ryan Pridgeon COM2032 rp00091

#ifndef RYAN_GLOBALS_H
#define RYAN_GLOBALS_H

// This scales down the distances of planets in the simulation. For example, if the scale is 0.00000001
// and a planet is 149600000km from the sun, it will be drawn at 1.496 units in 3d space from the sun
const float distanceScale = 0.00000001f;

// This scales the size of the planets in the simulation. For example, if the radius of a planet
// is 6,371km and the scale is 0.0001f, it will be drawn with a radius of 0.6371 units
extern float planetSizeScale;

#endif
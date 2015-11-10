/*Published under The MIT License (MIT)

See LICENSE.TXT*/

// Ryan Pridgeon COM2032 rp00091

#ifndef RYAN_PLANET_H
#define RYAN_PLANET_H

#ifdef _WIN32
#include <Windows.h>
#endif
#include <gl\GL.h>
#include <vector>
#include "moon.h"

class Planet
{
private:
	// distance from the sun
	float distanceFromSun;

	// time it takes to complete 1 orbit
	float orbitTime;

	// time it takes to spin 360 degrees
	float rotationTime;

	// radius of the planet itself
	float radius;

	// the texture used for rendering
	GLuint textureHandle;

	// its position in 3d space
	float position[3];
	// its rotation around its axis
	float rotation;

	// list of moons attached to this planet
	std::vector<Moon> moons;

public:
	// Distance is in units of km (kilometers) and time is in units of earth days (365.25 orbit time for earth)
	Planet(float distanceFromSun, float orbitTime, float rotationTime, float radius, GLuint textureHandle);

	// Calculate its position in 3d spacein the orbit using the given time value
	void calculatePosition(float time);

	// Render it to the screen
	void render(void);

	// render this planets orbit circle
	void renderOrbit(void);

	// Get its position in 3d world space units (after scaling) and put it into the 3d vector
	void getPosition(float* vec);

	// get the radius of this planet
	float getRadius(void);

	// add a moon to this planet
	void addMoon(float distanceFromPlanet, float orbitTime, float rotationTime, float radius, GLuint textureHandle);
};

#endif
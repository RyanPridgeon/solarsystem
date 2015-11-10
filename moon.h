/*Published under The MIT License (MIT)

See LICENSE.TXT*/

// Ryan Pridgeon COM2032 rp00091

#ifndef RYAN_MOON_H
#define RYAN_MOON_H

#ifdef _WIN32
#include <Windows.h>
#endif
#include <gl\GL.h>

class Moon
{
private:
	// distance from its planet
	float distanceFromPlanet;

	// time it takes to complete 1 orbit
	float orbitTime;

	// time it takes to spin 360 degrees
	float rotationTime;

	// radius of the moon itself
	float radius;

	// the texture used for rendering
	GLuint textureHandle;

	// its position in 3d space relative to the planet
	float position[3];
	// its rotation around its axis
	float rotation;

public:
	// Distance is in units of km (kilometers) and time is in units of earth days
	Moon(float distanceFromPlanet, float orbitTime, float rotationTime, float radius, GLuint textureHandle);

	// Calculate its position in 3d space relative to the planet in the orbit using the given time value
	void calculatePosition(float time);

	// Render it to the screen
	void render(void);

	// render this moons orbit circle
	void renderOrbit(void);
};

#endif
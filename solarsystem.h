/*Published under The MIT License (MIT)

See LICENSE.TXT*/

// Ryan Pridgeon COM2032 rp00091

#ifndef RYAN_SOLARSYSTEM_H
#define RYAN_SOLARSYSTEM_H

#ifdef _WIN32
#include <Windows.h>
#endif
#include <gl\GL.h>
#include <vector>

#include "planet.h"

class SolarSystem
{
private:
	std::vector<Planet> planets;

public:
	SolarSystem();

	// calculate the positions and logic of all planets
	void calculatePositions(float time);
	
	// Add a planet with the given data
	void addPlanet(float distanceFromSun, float orbitTime, float rotationTime, float radius, GLuint textureHandle);

	// Add a moon to the specified planet
	void addMoon(int planetIndex, float distanceFromPlanet, float orbitTime, float rotationTime, float radius, GLuint textureHandle);

	// render the planets with opengl
	void render();

	// render the drawing of the orbits
	void renderOrbits();

	// get the position in 3d space units of the given planet (specified by its index in the list) and put it into the 3d vector vec
	void getPlanetPosition(int index, float* vec);

	// get the radius of the planet at the given index in the planets list
	float getRadiusOfPlanet(int index);
};

#endif
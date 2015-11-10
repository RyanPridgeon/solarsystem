/*Published under The MIT License (MIT)

See LICENSE.TXT*/

// Ryan Pridgeon COM2032 rp00091

#include "solarsystem.h"

SolarSystem::SolarSystem()
{
	
}

// calculate the positions and logic of all planets
void SolarSystem::calculatePositions(float time)
{
	for (int i = 0; i < planets.size(); i++)
	{
		planets[i].calculatePosition(time);
	}
}

// Add a planet with the given data
void SolarSystem::addPlanet(float distanceFromSun, float orbitTime, float rotationTime, float radius, GLuint textureHandle)
{
	planets.push_back(Planet(distanceFromSun, orbitTime, rotationTime, radius, textureHandle));
}

// Add a moon to the specified planet
void SolarSystem::addMoon(int planetIndex, float distanceFromPlanet, float orbitTime, float rotationTime, float radius, GLuint textureHandle)
{
	planets[planetIndex].addMoon(distanceFromPlanet, orbitTime, rotationTime, radius, textureHandle);
}

// render the planets with opengl
void SolarSystem::render()
{
	for (int i = 0; i < planets.size(); i++)
	{
		planets[i].render();
	}
}


// render the drawing of the orbits
void SolarSystem::renderOrbits()
{
	for (int i = 0; i < planets.size(); i++)
	{
		planets[i].renderOrbit();
	}
}

// get the position in 3d space units of the given planet (specified by its index in the list) and put it into the 3d vector vec
void SolarSystem::getPlanetPosition(int index, float* vec)
{
	planets[index].getPosition(vec);
}


// get the radius of the planet at the given index in the planets list
float SolarSystem::getRadiusOfPlanet(int index)
{
	return planets[index].getRadius();
}
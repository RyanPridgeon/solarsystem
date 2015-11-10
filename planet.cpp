/*Published under The MIT License (MIT)

See LICENSE.TXT*/

// Ryan Pridgeon COM2032 rp00091

#include "planet.h"
#include <cmath>

#ifdef _WIN32
#include <Windows.h>
#endif
#include <gl\GL.h>
#include <gl\GLU.h>
#include <gl\glut.h>
#include "globals.h"

float planetSizeScale = 0.000005f;

Planet::Planet(float distanceFromSun, float orbitTime, float rotationTime, float radius, GLuint textureHandle)
{
	this->distanceFromSun = distanceFromSun;
	this->orbitTime = orbitTime;
	this->rotationTime = rotationTime;
	this->radius = radius;
	this->textureHandle = textureHandle;
}

// Calculate its position in 3d spacein the orbit using the given time value
void Planet::calculatePosition(float time)
{
	// find the angle of orientation of the orbit around the sun
	float angle = time * 3.1419f / orbitTime;
	
	// use trig to find the position in space
	position[0] = sin(angle) * distanceFromSun;
	position[1] = cos(angle) * distanceFromSun;
	position[2] = 0;

	// find the rotation of the planet around its axis
	rotation = time * 360 / rotationTime;

	// calculate positions of moons
	for (int i = 0; i < moons.size(); i++)
	{
		moons[i].calculatePosition(time);
	}
}

// Render it to the screen
void Planet::render(void)
{
	glPushMatrix();

	// translate to the right positon
	glTranslatef(position[0] * distanceScale, position[1] * distanceScale, position[2] * distanceScale);

	// Draw the moons
	for (int i = 0; i < moons.size(); i++)
	{
		moons[i].render();
	}

	/// rotate for the planet's spin
	glRotatef(rotation, 0.0f, 0.0f, 1.0f);
	
	// bind the planets texture
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	
	// render as a GLU sphere quadric object
	GLUquadricObj* quadric = gluNewQuadric();
	gluQuadricTexture(quadric, true);
	gluQuadricNormals(quadric, GLU_SMOOTH);

	if (distanceFromSun < 0.001f) // if this is the sun, dont render it too big, and disable lighting
	{
		float radiusScaled = radius * planetSizeScale;
		if (radiusScaled > 0.5f) radiusScaled = 0.5f;

		glDisable(GL_LIGHTING);
		gluSphere(quadric, radiusScaled, 30, 30);
		glEnable(GL_LIGHTING);
	}
	else
	{
		gluSphere(quadric, radius * planetSizeScale, 30, 30);
	}


	glPopMatrix();
}

// render this planets orbit circle
void Planet::renderOrbit(void)
{
	// draw a line strip
	glBegin(GL_LINE_STRIP);

	// loop round from 0 to 2*PI and draw around the radius of the orbit using trigonometry
	for (float angle = 0.0f; angle < 6.283185307f; angle += 0.05f)
	{
		glVertex3f(sin(angle) * distanceFromSun * distanceScale, cos(angle) * distanceFromSun * distanceScale, 0.0f);
	}
	glVertex3f(0.0f, distanceFromSun * distanceScale, 0.0f);
	
	glEnd();

	// render the moons' orbit
	glPushMatrix();
	// translate to the center of this planet to draw the moon orbit around it
	glTranslatef(position[0] * distanceScale, position[1] * distanceScale, position[2] * distanceScale);
	// draw all moon orbits
	for (int i = 0; i < moons.size(); i++)
	{
		moons[i].renderOrbit();
	}
	glPopMatrix();

}


// Get its position in 3d world space units (after scaling) and put it into the 3d vector
void Planet::getPosition(float* vec)
{
	vec[0] = position[0] * distanceScale;
	vec[1] = position[1] * distanceScale;
	vec[2] = position[2] * distanceScale;
}

// get the radius of this planet
float Planet::getRadius(void)
{
	return radius;
}

// add a moon to this planet
void Planet::addMoon(float distanceFromPlanet, float orbitTime, float rotationTime, float radius, GLuint textureHandle)
{
	moons.push_back(Moon(distanceFromPlanet, orbitTime, rotationTime, radius, textureHandle));
}
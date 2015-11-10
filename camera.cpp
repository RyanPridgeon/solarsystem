/*Published under The MIT License (MIT)

See LICENSE.TXT*/

// Ryan Pridgeon COM2032 rp00091

#ifdef _WIN32
#include <Windows.h>
#endif
#include <gl\GL.h>
#include <gl\GLU.h>
#include <cmath>
#include "camera.h"


// my camera system i envisioned to be controlled kinda like a spaceship
// so the keyboard would manipulate roll and direction and move around
// so im representing it with 4 vectors
// an up, forward, and right vector to represent orientation on all axes
// and a position vector to represent the translation

// these are some vector funcs i made to help me use the vectors easily

// sets vec to (x,y,z)
void vectorSet(float* vec, float x, float y, float z)
{
	vec[0] = x;
	vec[1] = y;
	vec[2] = z;
}

// adds v2 to v1
void vectorAdd(float* v1, float* v2)
{
	v1[0] += v2[0];
	v1[1] += v2[1];
	v1[2] += v2[2];
}

// copies v2 into v1
void vectorCopy(float* v1, float* v2)
{
	v1[0] = v2[0];
	v1[1] = v2[1];
	v1[2] = v2[2];
}

// multiplies vec by the scalar
void vectorMul(float* vec, float scalar)
{
	vec[0] *= scalar;
	vec[1] *= scalar;
	vec[2] *= scalar;
}

// finds the magnitude of a vec using pythag
float lengthOfVec(float* vec)
{
	return sqrtf(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
}

// normalises a vector to magnitude 1
void normaliseVec(float* vec)
{
	vectorMul(vec, 1 / lengthOfVec(vec));
}

// makes a 3x3 rotation matrix from the given angle and axis and pointer to a 3x3 matrix
void rotationMatrix(float* matrix, float* axis, float angle)
{
	// using http://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
	float cos1 = cos(angle);
	float cos2 = 1 - cos1;
	float sin1 = sin(angle);

	matrix[0] = cos1 + axis[0]*axis[0]*cos2;
	matrix[1] = axis[0] * axis[1] * cos2 - axis[2]*sin1;
	matrix[2] = axis[0] * axis[2] * cos2 + axis[1]*sin1;

	matrix[3] = axis[1] * axis[0] * cos2 + axis[2]*sin1;
	matrix[4] = cos1 + axis[1] * axis[1] * cos2;
	matrix[5] = axis[1] * axis[2] * cos2 - axis[0] * sin1;
	
	matrix[6] = axis[2] * axis[0] * cos2 - axis[1] * sin1;
	matrix[7] = axis[2] * axis[1] * cos2 + axis[0] * sin1;
	matrix[8] = cos1 + axis[2] * axis[2] * cos2;
}

// multiplies a vector v1 by a matrix and puts the results into vector v2
void mulVecBy(float* v1, float* matrix, float* v2)
{
	v2[0] = v1[0] * matrix[0] + v1[1] * matrix[1] + v1[2] * matrix[2];
	v2[1] = v1[0] * matrix[3] + v1[1] * matrix[4] + v1[2] * matrix[5];
	v2[2] = v1[0] * matrix[6] + v1[1] * matrix[7] + v1[2] * matrix[8];
}

// rotate a vector v1 around the axis v2 by angle and put the result into v3
void rotateAroundVec(float* v1, float* v2, float angle, float* v3)
{
	//. make a rotation matrix for it
	float matrix[16];
	rotationMatrix(matrix, v2, angle);

	// multiply by the matrix
	mulVecBy(v1, matrix, v3);
}

Camera::Camera(void)
{
	cameraSpeed = 0.005f;
	cameraTurnSpeed = 0.01f;
	// set default vector values - obtained a nice viewing angle of the planets, got values using the debugger
	/*		forwardVec	0x00ab354c {-0.398769796, 0.763009906, -0.508720219}	float[3]
+		upVec	0x00ab3564 {-0.235630989, 0.450859368, 0.860931039}	float[3]
+		rightVec	0x00ab3558 {0.886262059, 0.463184059, 0.000000000}	float[3]
+		position	0x00ab3570 {0.764331460, -1.66760659, 0.642456770}	float[3] */
	vectorSet(position, 0.764331460f, -1.66760659f, 0.642456770);
	vectorSet(forwardVec,-0.398769796f, 0.763009906f, -0.508720219f);
	vectorSet(rightVec, 0.886262059f, 0.463184059f, 0.000000000f);
	vectorSet(upVec, -0.235630989f, 0.450859368f, 0.860931039f);
}

// transform the opengl view matrix for the camera
void Camera::transformOrientation(void)
{
	// look in the direction of the orientation vectors
	gluLookAt(0, 0, 0, forwardVec[0], forwardVec[1], forwardVec[2], upVec[0], upVec[1], upVec[2]);
}

// transform the opoengl view matrix for the translation
void Camera::transformTranslation(void)
{
	// translate to emulate camera position
	glTranslatef(-position[0], -position[1], -position[2]);
}

// points the camera at the given point in 3d space
void Camera::pointAt(float* targetVec)
{
	float tempVec[3];
	float up[3] = { 0.0f, 0.0f, 1.0f };

	// first work out the new forward vector by subtracting the target position from the camera position
	forwardVec[0] = targetVec[0] - position[0];
	forwardVec[1] = targetVec[1] - position[1];
	forwardVec[2] = targetVec[2] - position[2];
	// then normalise it to 1 length
	normaliseVec(forwardVec);

	// now to find the right vector we rotate the forward vector -pi/2 around the z axis
	rotateAroundVec(forwardVec, up, -1.57079632679f, tempVec);
	// and remove the y component to make it flat
	tempVec[2] = 0;
	// then normalise it
	normaliseVec(tempVec);
	// and assign it to rightVec
	vectorCopy(rightVec, tempVec);

	// now work out the upvector by rotating the forward vector pi/2 around the rightvector
	rotateAroundVec(forwardVec, rightVec, 1.57079632679f, tempVec);
	vectorCopy(upVec, tempVec);
}

// speed up the camera speed
void Camera::speedUp(void)
{
	if (cameraSpeed < 1.0f)
		cameraSpeed *= 2;
}

// slow down the camera speed
void Camera::slowDown(void)
{
	if (cameraSpeed > 0.000001f)
		cameraSpeed /= 2;
}

// move the camera forward
void Camera::forward(void)
{
	// make a movement vector the right speed facing the forward direction
	float vec[3];
	vectorCopy(vec, forwardVec);
	vectorMul(vec, cameraSpeed);

	// add the movement vec to the position vec
	vectorAdd(position, vec);
}

// move the camera backward
void Camera::backward(void)
{
	// make a movement vector the right speed facing the backward direction
	float vec[3];
	vectorCopy(vec, forwardVec);
	vectorMul(vec, -cameraSpeed); // -cameraSpeed for backwards

	// add the movement vec to the position vec
	vectorAdd(position, vec);
}

// strafe left
void Camera::left(void)
{
	// make a movement vector the right speed facing the left direction
	float vec[3];
	vectorCopy(vec, rightVec);
	vectorMul(vec, -cameraSpeed); // -cameraSpeed for left

	// add the movement vec to the position vec
	vectorAdd(position, vec);
}

// strafe right
void Camera::right(void)
{
	// make a movement vector the right speed facing the right direction
	float vec[3];
	vectorCopy(vec, rightVec);
	vectorMul(vec, cameraSpeed);

	// add the movement vec to the position vec
	vectorAdd(position, vec);
}

// roll the camera to the right
void Camera::rollRight(void)
{
	float tempVec[3];
	// rotate the up and right vectors around the forward vector axis for roll
	rotateAroundVec(upVec, forwardVec, cameraTurnSpeed, tempVec);
	vectorCopy(upVec, tempVec);
	
	rotateAroundVec(rightVec, forwardVec, cameraTurnSpeed, tempVec);
	vectorCopy(rightVec, tempVec);
}

// roll the camera to the left
void Camera::rollLeft(void)
{
	float tempVec[3];
	// rotate the up and right vectors around the forward vector axis for roll
	rotateAroundVec(upVec, forwardVec, -cameraTurnSpeed, tempVec);
	vectorCopy(upVec, tempVec);
	
	rotateAroundVec(rightVec, forwardVec, -cameraTurnSpeed, tempVec);
	vectorCopy(rightVec, tempVec);
}

// pitch the camera up
void Camera::pitchUp(void)
{
	float tempVec[3];
	// rotate the forward and up vectors around the right vector axis for pitch
	rotateAroundVec(forwardVec, rightVec, cameraTurnSpeed, tempVec);
	vectorCopy(forwardVec, tempVec);
	
	rotateAroundVec(upVec, rightVec, cameraTurnSpeed, tempVec);
	vectorCopy(upVec, tempVec);
}

// pitch the camera down
void Camera::pitchDown(void)
{
	float tempVec[3];
	// rotate the forward and up vectors around the right vector axis for pitch
	rotateAroundVec(forwardVec, rightVec, -cameraTurnSpeed, tempVec);
	vectorCopy(forwardVec, tempVec);
	
	rotateAroundVec(upVec, rightVec, -cameraTurnSpeed, tempVec);
	vectorCopy(upVec, tempVec);
}

// yaw left
void Camera::yawLeft(void)
{
	float tempVec[3];
	// rotate the forward and right vectors around the up vector axis for yaw
	rotateAroundVec(forwardVec, upVec, cameraTurnSpeed, tempVec);
	vectorCopy(forwardVec, tempVec);
	
	rotateAroundVec(rightVec, upVec, cameraTurnSpeed, tempVec);
	vectorCopy(rightVec, tempVec);
}

// yaw right
void Camera::yawRight(void)
{
	float tempVec[3];
	// rotate the forward and right vectors around the up vector axis for yaw
	rotateAroundVec(forwardVec, upVec, -cameraTurnSpeed, tempVec);
	vectorCopy(forwardVec, tempVec);
	
	rotateAroundVec(rightVec, upVec, -cameraTurnSpeed, tempVec);
	vectorCopy(rightVec, tempVec);
}
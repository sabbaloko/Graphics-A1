//#ifndef CAMERA_H
//#define CAMERA_H
//
//#include "TTK\Vector3.h"
//#include "TTK\Matrix3x3.h"
//#include <GLUT\glut.h>
//
//class Camera{
//public:
//	//gluLookAt
//	TTK::Vector3 eye;
//	TTK::Vector3 center;
//	TTK::Vector3 up;
//
//	float pitch;
//	float yaw;
//
//	TTK::Vector3 forwardVec;
//	TTK::Vector3 forwardVecCopy;
//	float forwardVelocity;
//	float tangentVelocity;
//
//	bool heldW;
//	bool heldS;
//	bool heldA;
//	bool heldD;
//
//	Camera( float ex = 0.0, float ey = 0.0, float ez = 0.0,	// default camera sits at the origin, looking down the Z axis
//			float cx = 0.0, float cy = 0.0, float cz = 1.0,
//			float ux = 0.0, float uy = 1.0, float uz = 0.0)
//	{
//		eye.x = ex;
//		eye.y = ey;
//		eye.z = ez;
//		center.x = cx;
//		center.y = cy;
//		center.z = cz;
//		up.x = ux;
//		up.y = uy;
//		up.z = uz;
//
//		pitch = 0.0f;
//		yaw = 0.0f;
//
//		forwardVec.x = cx - ex;
//		forwardVec.y = cy - ey;
//		forwardVec.z = cz - ez;
//		forwardVec = TTK::Vector3::normalize(forwardVec);
//
//		forwardVecCopy = forwardVec;
//		forwardVelocity = 0.0;
//		tangentVelocity = 0.0;
//
//		heldW = false;
//		heldS = false;
//		heldA = false;
//		heldD = false;
//	}
//
//	void Update(float deltaT)
//	{
//		TTK::Matrix3x3 rotx = TTK::Matrix3x3::CreateRotationX(pitch);
//		TTK::Matrix3x3 roty = TTK::Matrix3x3::CreateRotationY(yaw);
//
//		forwardVec = rotx * roty * forwardVecCopy;
//
//
//		forwardVelocity = 0;
//		if (heldW) forwardVelocity += 15;
//		if (heldS) forwardVelocity -= 15;
//
//		tangentVelocity = 0;
//		if (heldA) tangentVelocity -= 5;
//		if (heldD) tangentVelocity += 5;
//
//		TTK::Vector3 velocity;
//
//		// apply forward velocity
//		velocity = forwardVec * forwardVelocity;
//
//		// apply tangent velocity
//		TTK::Vector3 tangent = TTK::Vector3::cross(forwardVec, TTK::Vector3::up());
//		tangent = TTK::Vector3::normalize(tangent);
//		velocity = velocity + (tangent * tangentVelocity);
//
//		// recalulate local up vector based on forward and tangent
//		up = TTK::Vector3::cross(tangent, forwardVec);
//		up = TTK::Vector3::normalize(up);
//
//		// update position
//		eye = eye + (velocity * deltaT);
//
//		// recalculating what we're looking at
//		center = eye + forwardVec;
//	}
//
//	void applyCameraTransformations()
//	{
//		gluLookAt(
//				eye.x, eye.y, eye.z,
//				center.x, center.y, center.z,
//				up.x, up.y, up.z
//			);
//	}
//
//	void setPitch(float rad)
//	{
//		pitch += rad;
//	}
//
//	void setYaw(float rad)
//	{
//		yaw += rad;
//	}
//
//};
//
//#endif
#ifndef CAMERA_H
#define CAMERA_H

#include <vec3.hpp>
#include <mat3x3.hpp>
#include <mat4x4.hpp>
#include <GLUT\glut.h>
#include <glm.hpp>
#include <gtx\transform.hpp>

class Camera{
public:
	//gluLookAt
	glm::vec3 eye;
	glm::vec3 center;
	glm::vec3 up;

	float pitch;
	float yaw;

	glm::mat4x4 matrix;

	glm::vec3 forwardVec;
	glm::vec3 forwardVecCopy;
	float forwardVelocity;
	float tangentVelocity;

	glm::vec3 rotx = glm::normalize(rotx);
	glm::vec3 roty = glm::normalize(roty);

	bool heldW;
	bool heldS;
	bool heldA;
	bool heldD;

	Camera(float ex = 0.0, float ey = 0.0, float ez = 0.0,	// default camera sits at the origin, looking down the Z axis
		float cx = 0.0, float cy = 0.0, float cz = 1.0,
		float ux = 0.0, float uy = 1.0, float uz = 0.0)
	{
		eye.x = ex;
		eye.y = ey;
		eye.z = ez;
		center.x = cx;
		center.y = cy;
		center.z = cz;
		up.x = ux;
		up.y = uy;
		up.z = uz;

		pitch = 0.0f;
		yaw = 0.0f;

		forwardVec.x = cx - ex;
		forwardVec.y = cy - ey;
		forwardVec.z = cz - ez;
		forwardVec = glm::normalize(forwardVec);

		forwardVecCopy = forwardVec;
		forwardVelocity = 0.0;
		tangentVelocity = 0.0;

		heldW = false;
		heldS = false;
		heldA = false;
		heldD = false;
	}

	void Update(float deltaT)
	{
		glm::mat3x3 rotx;
		glm::mat3x3 roty;
		//Pitch and Yaw are angles, foreward vec is the axis, both are creating 
		//the rotation matix where we get rotation x and rotation y
		rotx = glm::mat3x3 ( glm::rotate(pitch, glm::cross(forwardVec, up)));
		roty = glm::mat3x3 ( glm::rotate(yaw, up));

		//glm::mat4x4 MatrixY = glm::rotate(
			
		//glm::mat4x4 rotx = glm::rotate(pitch, glm::cross(forwardVec, up));
		//glm::mat4x4 roty = glm::rotate(yaw, up);
		//glm::mat3x3 matx = glm::normalize(matx);
		//glm::mat3x3 maty = glm::normalize(maty);

		glm::rotate(pitch, forwardVec);
		glm::rotate(yaw, forwardVec);

		forwardVec = rotx * roty * forwardVecCopy;

		forwardVelocity = 0;
		if (heldW) forwardVelocity += 15;
		if (heldS) forwardVelocity -= 15;

		tangentVelocity = 0;
		if (heldA) tangentVelocity -= 5;
		if (heldD) tangentVelocity += 5;

		glm::vec3 velocity;

		// apply forward velocity
		velocity = forwardVec * forwardVelocity;

		// apply tangent velocity
		glm::vec3 tangent = glm::cross(forwardVec, up);
		tangent = glm::normalize(tangent);
		velocity = velocity + (tangent * tangentVelocity);

		// recalulate local up vector based on forward and tangent
		up = glm::cross(tangent, forwardVec);
		up = glm::normalize(up);

		// update position
		eye = eye + (velocity * deltaT);

		// recalculating what we're looking at
		center = eye + forwardVec;
	}

	void applyCameraTransformations()
	{
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			up.x, up.y, up.z
			);
	}

	void setPitch(float rad)
	{
		pitch += rad;
	}

	void setYaw(float rad)
	{
		yaw += rad;
	}

};

#endif

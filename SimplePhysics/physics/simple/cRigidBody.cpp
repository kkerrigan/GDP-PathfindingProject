/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#include <physics/interfaces/iPlaneShape.h>
#include <physics/interfaces/iSphereShape.h>

#include "cRigidBody.h"

nPhysics::cRigidBody::cRigidBody() : iRigidBody() {

	return;
}

nPhysics::cRigidBody::cRigidBody(const sRigidBodyDef& def, iShape* shape) {

	this->mMass = def.mass;
	this->mPosition = def.position;
	this->mPreviousPosition = def.position;
	this->mAcceleration = glm::vec3(0.0f, 0.0f, 0.0f);
	this->mOrientation = glm::quat(def.orientation);
	this->mVelocity = def.velocity;
	this->mShape = shape;

	this->mSetOrientationEulerAngles(def.orientation, true);

	return;
}

nPhysics::cRigidBody::cRigidBody(const cRigidBody & copy) : iRigidBody(copy) {

	return;
}

nPhysics::cRigidBody & nPhysics::cRigidBody::operator=(const cRigidBody & copy) {
	return *this;
}

nPhysics::cRigidBody::~cRigidBody() {

	return;
}

void nPhysics::cRigidBody::getPosition(glm::vec3& positionOut) {

	positionOut = this->mPosition;

	return;
}

void nPhysics::cRigidBody::setPosition(glm::vec3 position) {

	this->mPosition = position;

	return;
}

void nPhysics::cRigidBody::getPreviousPosition(glm::vec3& positionOut) {

	positionOut = this->mPreviousPosition;

	return;
}

void nPhysics::cRigidBody::setPreviousPosition(glm::vec3 position) {

	this->mPreviousPosition = position;

	return;
}

void nPhysics::cRigidBody::getOrientation(glm::quat& orientationOut) {

	orientationOut = this->mOrientation;

	return;
}

void nPhysics::cRigidBody::setOrientation(glm::quat orientation) {

	this->mOrientation = orientation;

	return;
}

void nPhysics::cRigidBody::getVelocity(glm::vec3& velocityOut) {

	velocityOut = this->mVelocity;

	return;
}

void nPhysics::cRigidBody::setVelocity(glm::vec3 velocity) {

	this->mVelocity = velocity;

	return;
}

void nPhysics::cRigidBody::getAcceleration(glm::vec3& accelerationOut) {

	accelerationOut = this->mAcceleration;

	return;
}

void nPhysics::cRigidBody::setAcceleration(glm::vec3 acceleration) {

	this->mAcceleration = acceleration;

	return;
}

void nPhysics::cRigidBody::getMass(float& massOut) {

	massOut = this->mMass;

	return;
}

void nPhysics::cRigidBody::setMass(float mass) {

	this->mMass = mass;

	return;
}

void nPhysics::cRigidBody::applyFoce(glm::vec3 forceAccel) {

	this->mAcceleration += forceAccel;

	return;
}

nPhysics::iShape* nPhysics::cRigidBody::getShape() {
	
	return this->mShape;
}

void nPhysics::cRigidBody::getAABB(glm::vec3& minOut, glm::vec3& maxOut) {

	if (this->getShape()->getShapeType() == nPhysics::eShapeType::SHAPE_TYPE_SPHERE) {

		glm::vec3 pos(0.0f);
		this->getPosition(pos);

		float radius = static_cast<nPhysics::iSphereShape*>(this->getShape())->getRadius();

		minOut = pos - glm::vec3(radius);
		maxOut = pos + glm::vec3(radius);
	}

	if (this->getShape()->getShapeType() == nPhysics::eShapeType::SHAPE_TYPE_PLANE) {

		glm::vec3 pos(0.0f);
		this->getPosition(pos);

		glm::vec3 normal = static_cast<nPhysics::iPlaneShape*>(this->getShape())->getPlaneNormal();
		glm::vec3 offset = glm::vec3(1.0f) - normal;

		minOut = pos - normal - offset * FLT_MAX;
		maxOut = pos + normal + offset * FLT_MAX;
	}

	return;
}

void nPhysics::cRigidBody::mSetOrientationEulerAngles(glm::vec3 newAnglesEuler, bool bIsDegrees) {

	if (bIsDegrees) {
		newAnglesEuler = glm::vec3(glm::radians(newAnglesEuler.x), glm::radians(newAnglesEuler.y), glm::radians(newAnglesEuler.z));
	}

	this->mOrientation = glm::quat(glm::vec3(newAnglesEuler.x, newAnglesEuler.y, newAnglesEuler.z));
	return;
}

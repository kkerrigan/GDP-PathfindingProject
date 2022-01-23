/**
 * @file cFleeBehaviour.cpp
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Implementation of the cFleeBehaviour class.
 */

#include <iostream>

#include "cFleeBehaviour.h"
#include "Common.h"
#include "cRigidBodyComponent.h"

cFleeBehaviour::cFleeBehaviour(cEntity* fleeEntity, cEntity* targetEntity) {

	this->mFleeEntity = fleeEntity;
	this->mTargetEntity = targetEntity;

	return;
}

cFleeBehaviour::~cFleeBehaviour() {

	return;
}

void cFleeBehaviour::update(cEntity* pEntity, float deltaTime) {

	cRigidBodyComponent* fleePhysicsComp = this->mFleeEntity->getComponent<cRigidBodyComponent>();
	cRigidBodyComponent* targetPhysicsComp = this->mTargetEntity->getComponent<cRigidBodyComponent>();

	if (fleePhysicsComp == nullptr || targetPhysicsComp == nullptr) {

		std::cout << "One or both entities in a cFleeComponent does not have a PhysicsComponent!" << std::endl;
		return;
	}

	// Get the rigid bodies for each entity
	nPhysics::iRigidBody* fleeBody = fleePhysicsComp->getRigidBody();
	nPhysics::iRigidBody* targetBody = targetPhysicsComp->getRigidBody();

	// Find forward direction
	glm::quat fleeOri = glm::quat();
	fleeBody->getOrientation(fleeOri);
	glm::vec3 forwardVec = fleeOri * FORWARD;

	// Find flee direction
	glm::vec3 fleePos(0.0f);
	glm::vec3 targetPos(0.0f);
	
	fleeBody->getPosition(fleePos);
	targetBody->getPosition(targetPos);
	glm::vec3 fleeDirection = glm::normalize(fleePos - targetPos);

	// Flee code
	glm::quat orientation = glm::quat(glm::lookAt(fleePos, fleeDirection, UP));

	// Update fleeing entities orientation
	fleeBody->setOrientation(orientation);

	// update fleeing entities velocity
	glm::vec3 updatedVel(0.0f);
	updatedVel.x = fleeDirection.x * 5.0f;
	updatedVel.z = fleeDirection.z * 5.0f;
	fleeBody->setVelocity(updatedVel);

	return;
}

bool cFleeBehaviour::isFinished() {
	
	return false;
}

void cFleeBehaviour::startBehaviour()
{
}

void cFleeBehaviour::stopBehaviour()
{
}

bool cFleeBehaviour::isStarted()
{
	return false;
}

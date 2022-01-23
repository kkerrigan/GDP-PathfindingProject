/**
 * @file cSeekBehaviour.cpp
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Implementation of the cSeekBehaviour class.
 */

#include <iostream>
#include <gameMath.h>

#include "cSeekBehaviour.h"
#include "Common.h"
#include "cRigidBodyComponent.h" // Use the position from this rather than TranformComponent because of collisions

cSeekBehaviour::cSeekBehaviour(cEntity* seeker, cEntity* target) {

	this->mSeeker = seeker;
	this->mTarget = target;

	return;
}

cSeekBehaviour::~cSeekBehaviour() {

	return;
}

void cSeekBehaviour::update(cEntity* pEntity, float deltaTime) {

	// Get the physics components for each entity
	cRigidBodyComponent* seekerPhysicsComp = this->mSeeker->getComponent<cRigidBodyComponent>();
	cRigidBodyComponent* targetPhysicsComp = this->mTarget->getComponent<cRigidBodyComponent>();

	if (seekerPhysicsComp == nullptr || targetPhysicsComp == nullptr) {

		std::cout << "One of the entities on a cSeekBehaviour had no physics component!" << std::endl;
		return;
	}

	// Get the rigid bodies for the components
	nPhysics::iRigidBody* seekerBody = seekerPhysicsComp->getRigidBody();
	nPhysics::iRigidBody* targetBody = targetPhysicsComp->getRigidBody();

	// Find forward direction
	glm::quat seekerOri = glm::quat();
	glm::vec3 forwardVec(0.0f);
	seekerBody->getOrientation(seekerOri);

	// Find the seek direction
	glm::vec3 seekerPos(0.0f);
	glm::vec3 targetPos(0.0f);

	seekerBody->getPosition(seekerPos);
	targetBody->getPosition(targetPos);
	glm::vec3 seekVec = glm::normalize(targetPos - seekerPos);

	// Seek code
	glm::quat orientation = glm::quat(glm::lookAt(seekerPos, seekVec, UP));
	seekerBody->setOrientation(orientation);
	
	// Update seeker velocity
	glm::vec3 updatedVel(0.0f);
	
	updatedVel.x = seekVec.x * 5.0f;
	updatedVel.z = seekVec.z * 5.0f;

	seekerBody->setVelocity(updatedVel);

	return;
}

bool cSeekBehaviour::isFinished() {
	
	return false;
}

void cSeekBehaviour::startBehaviour() {

	return;
}

void cSeekBehaviour::stopBehaviour() {

	return;
}

bool cSeekBehaviour::isStarted() {
	
	return false;
}

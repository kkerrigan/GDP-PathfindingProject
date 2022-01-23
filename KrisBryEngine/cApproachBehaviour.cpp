/**
 * @file cApproachBehaviour.cpp
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Implementation of the cApproachBehaviour.cpp.
 */

#include <iostream>

#include "cApproachBehaviour.h"
#include "Common.h"
#include "cRigidBodyComponent.h"

cApproachBehaviour::cApproachBehaviour(cEntity* pursuer, cEntity* target, float minPursueDist) {

	this->mPursuer = pursuer;
	this->mTarget = target;
	this->mMinPursueDistance = minPursueDist;

	return;
}


cApproachBehaviour::~cApproachBehaviour() {

	return;
}


void cApproachBehaviour::update(cEntity* pEntity, float deltaTime) {

	// Get the physics components from the entities
	cRigidBodyComponent* pursuerPhysicsComp = this->mPursuer->getComponent<cRigidBodyComponent>();
	cRigidBodyComponent* targetPhysicsComp = this->mTarget->getComponent<cRigidBodyComponent>();

	if (pursuerPhysicsComp == nullptr || targetPhysicsComp == nullptr) {

		std::cout << "One or more of the entities in a cPursueBehaviour does not have a physics component!" << std::endl;
		return;
	}

	nPhysics::iRigidBody* pursuerBody = pursuerPhysicsComp->getRigidBody();
	nPhysics::iRigidBody* targetBody = targetPhysicsComp->getRigidBody();

	// Find the distance between the two entities
	glm::vec3 pursuerPos(0.0f);
	glm::vec3 targetPos(0.0f);

	pursuerBody->getPosition(pursuerPos);
	targetBody->getPosition(targetPos);

	float distance = glm::abs(glm::distance(targetPos, pursuerPos));
	if (distance < this->mMinPursueDistance) {

		// Reached min distance stop the pursuer
		pursuerBody->setVelocity(glm::vec3(0.0f));
		return;
	}

	// Move towards the player
	glm::vec3 moveDirection = glm::normalize(targetPos - pursuerPos);

	glm::quat pursuerOri = glm::quat();
	pursuerBody->getOrientation(pursuerOri);

	glm::quat orientation = glm::quat(glm::lookAt(pursuerPos, moveDirection, UP));
	pursuerBody->setOrientation(orientation);

	// Update pursuer velocity
	glm::vec3 updatedVel(0.0f);

	updatedVel.x = moveDirection.x * 5.0f;
	updatedVel.z = moveDirection.z * 5.0f;
	pursuerBody->setVelocity(updatedVel);

	return;
}


bool cApproachBehaviour::isFinished() {

	return false;
}
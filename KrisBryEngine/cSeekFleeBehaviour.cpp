/**
 * @file cSeekFleeBehaviour.cpp
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Implementation of the cSeekFleeBehaviour class.
 */

#include <iostream>

#include "Common.h"
#include "cSeekFleeBehaviour.h"
#include "cSeekBehaviour.h"
#include "cFleeBehaviour.h"
#include "cRigidBodyComponent.h"

cSeekFleeBehaviour::cSeekFleeBehaviour(cEntity* agent, cEntity* target) {

	this->mAgent = agent;
	this->mTarget = target;

	return;
}

cSeekFleeBehaviour::~cSeekFleeBehaviour() {

	return;
}

void cSeekFleeBehaviour::update(cEntity* pEntity, float deltaTime) {

	// Get the Physics Component off the entities
	cRigidBodyComponent* agentPhysicsComp = this->mAgent->getComponent<cRigidBodyComponent>();
	cRigidBodyComponent* targetPhysicsComp = this->mTarget->getComponent<cRigidBodyComponent>();

	if (agentPhysicsComp == nullptr || targetPhysicsComp == nullptr) {

		std::cout << "One or more entities in a cSeekFleeBehaviour does not have a physics component!" << std::endl;
		return;
	}

	nPhysics::iRigidBody* agentBody = agentPhysicsComp->getRigidBody();
	nPhysics::iRigidBody* targetBody = targetPhysicsComp->getRigidBody();

	// Find the forward vector of the target
	glm::quat targetOri = glm::quat();
	glm::vec3 forwardVec(0.0f);

	targetBody->getOrientation(targetOri);
	forwardVec = targetOri * FORWARD;

	// Calculate the direction of the target
	glm::vec3 agentPos(0.0f);
	glm::vec3 targetPos(0.0f);
	glm::vec3 direction(0.0f);
	
	agentBody->getPosition(agentPos);
	targetBody->getPosition(targetPos);
	direction = glm::normalize(targetPos - agentPos);

	// Calculate the angle that the target is facing
	float angle = glm::dot(forwardVec, direction);

	if (angle > 0.0f) {

		// A greater than zero vector means the angle is less than 90 degrees (facing each other)
		cFleeBehaviour flee(this->mAgent, this->mTarget);
		flee.update(this->mAgent, deltaTime);
	}
	else {
		cSeekBehaviour seek(this->mAgent, this->mTarget);
		seek.update(this->mAgent, deltaTime);
	}

	return;
}

bool cSeekFleeBehaviour::isFinished() {
	
	return false;
}

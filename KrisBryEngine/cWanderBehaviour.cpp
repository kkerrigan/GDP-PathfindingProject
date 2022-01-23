/**
 * @file cWanderBehaviour.cpp
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Implementation of the cWanderBehaviour class.
 */

#include <iostream>

#include "Global.h"
#include "Common.h"
#include "cWanderBehaviour.h"
#include "cRigidBodyComponent.h"

cWanderBehaviour::cWanderBehaviour(cEntity* agent, float mapExtents) {

	this->mAgent = agent;
	this->mMapExtents = mapExtents;

	// Find the first position and set it as the target
	this->findNewPosition(this->mMapExtents);

	return;
}

cWanderBehaviour::~cWanderBehaviour() {

	return;
}

void cWanderBehaviour::update(cEntity* pEntity, float deltaTime) {

	// Get the physics component of the entity
	cRigidBodyComponent* agentPhysicsComp = this->mAgent->getComponent<cRigidBodyComponent>();
	if (agentPhysicsComp == nullptr) {

		std::cout << "The entity of a cWanderBehaviour does not have a physics component!" << std::endl;
		return;
	}

	nPhysics::iRigidBody* agentBody = agentPhysicsComp->getRigidBody();

	// Check if we have reached the target
	glm::vec3 agentPos(0.0f);
	agentBody->getPosition(agentPos);

	float distance = glm::abs(glm::distance(this->mTargetPosition, agentPos));
	if (distance <= 0.5f) {

		agentBody->setVelocity(glm::vec3(0.0f));
		return;
	}

	// Otherwise move towards that position
	glm::vec3 moveDirection = glm::normalize(this->mTargetPosition - agentPos);

	glm::quat agentOri = glm::quat();
	agentBody->getOrientation(agentOri);
	
	glm::quat orientation = glm::quat(glm::lookAt(agentPos, moveDirection, UP));
	agentBody->setOrientation(orientation);

	// Update the agents velocity
	glm::vec3 updatedVel(0.0f);
	updatedVel.x = moveDirection.x * 7.0f;
	updatedVel.z = moveDirection.z * 7.0f;

	agentBody->setVelocity(updatedVel);

	return;
}

bool cWanderBehaviour::isFinished() {
	
	cRigidBodyComponent* agentPhysicsComp = this->mAgent->getComponent<cRigidBodyComponent>();
	nPhysics::iRigidBody* agentBody = agentPhysicsComp->getRigidBody();

	// Check if we have reached the target
	glm::vec3 agentPos(0.0f);
	agentBody->getPosition(agentPos);

	float distance = glm::abs(glm::distance(this->mTargetPosition, agentPos));

	return distance <= 0.5f;
}

void cWanderBehaviour::startBehaviour()
{
}

void cWanderBehaviour::stopBehaviour()
{
}

bool cWanderBehaviour::isStarted()
{
	return false;
}

void cWanderBehaviour::findNewPosition(float extents) {
	
	float xPos = getRandInRange<float>(-this->mMapExtents, this->mMapExtents);
	float zPos = getRandInRange<float>(-this->mMapExtents, this->mMapExtents);
	
	this->mTargetPosition = glm::vec3(xPos, 0.0f, zPos);

	return;
}

/**
 * @file cFlockBehaviour.cpp
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 *
 * @section DESCRIPTION
 *
 * Implementation of the cFlockBehaviour class.
 */

#include "cFlockBehaviour.h"
#include "Common.h"

cFlockBehaviour::cFlockBehaviour() {

	this->mBIsBehaviourStarted = false;

	return;
}

cFlockBehaviour::~cFlockBehaviour() {


	return;
}

void cFlockBehaviour::update(cEntity* pEntity, float deltaTime) {

	this->mCalculateBoidCohesion(pEntity, this->mCohesion);

	this->mSeparation = glm::vec3(0.0f);
	this->mCalculateBoidSeparation(pEntity, this->mSeparation);

	this->mCalculateBoidAlignment(pEntity, this->mAlignment);

	// Calculate final steering vector
	cTransformComponent* pBoidTransform = pEntity->getComponent<cTransformComponent>();
	glm::vec3 forwardVec = glm::toMat3(pBoidTransform->getQOrientation()) * FORWARD;
	
	this->flockDirection = this->mCohesion + this->mSeparation + this->mAlignment;

	// Make sure the direction isn't zero and that it has changed since the last frame
	if (glm::length(this->flockDirection) == 0.0f || this->flockDirection != this->flockDirection) {

		this->flockDirection = forwardVec;
	}
	else {

		// Normalize the steering vector in case the weights sum to greater than 1
		glm::normalize(this->flockDirection);
	}

	return;
}

bool cFlockBehaviour::isFinished() {

	// Not implemented on this behaviour

	return true;
}

void cFlockBehaviour::startBehaviour() {

	this->mBIsBehaviourStarted = true;

	return;
}

void cFlockBehaviour::stopBehaviour() {

	this->mBIsBehaviourStarted = false;

	return;
}

bool cFlockBehaviour::isStarted() {
	
	return this->mBIsBehaviourStarted;
}

void cFlockBehaviour::addBoid(sBoid boid) {

	this->mBoidsInFlock.push_back(boid);

	return;
}

std::vector<cFlockBehaviour::sBoid>& cFlockBehaviour::getFlockMembers() {
	
	return this->mBoidsInFlock;
}

void cFlockBehaviour::mCalculateBoidCohesion(cEntity* boid, glm::vec3& cohesion) {

	cTransformComponent* pBoidTransform = boid->getComponent<cTransformComponent>();
	glm::vec3 forwardVec = glm::toMat3(pBoidTransform->getQOrientation()) * FORWARD;
	size_t totalBoids = this->mBoidsInFlock.size();

	// Calculate the global cohesion of the flock
	// Start by setting cohesion the the first boid
	cohesion = pBoidTransform->getPosition() + this->flockCenter;
	for (size_t index = 0; index < totalBoids; ++index)
	{
		this->mCohesion += this->mBoidsInFlock[index].transform->getPosition();
	}

	cohesion = (this->mCohesion - pBoidTransform->getPosition()) / static_cast<float>(totalBoids + 1) - pBoidTransform->getPosition();
	cohesion = glm::normalize(this->mCohesion) * this->cohesionWeight;


	return;
}

void cFlockBehaviour::mCalculateBoidSeparation(cEntity* boid, glm::vec3& separation) {
	
	cTransformComponent* pBoidTransform = boid->getComponent<cTransformComponent>();
	size_t totalBoids = this->mBoidsInFlock.size();

	// Check each boid to see if the separation should affect them (close enough)
	for (size_t index = 0; index < totalBoids; ++index) {

		if (glm::abs(glm::distance(this->mBoidsInFlock[index].transform->getPosition(), pBoidTransform->getPosition())) < FLOCK_RADIUS) {

			separation += glm::normalize(pBoidTransform->getPosition() - this->mBoidsInFlock[index].transform->getPosition());
		}
	}

	// Make sure the separation vector is greater than zero to avoid divide by zero
	if (glm::length(this->mSeparation) != 0.0f) {

		separation = glm::normalize(separation) * this->separationWeight;
	}

	return;
}

void cFlockBehaviour::mCalculateBoidAlignment(cEntity* boid, glm::vec3& alignment) {

	cTransformComponent* pBoidTransform = boid->getComponent<cTransformComponent>();
	glm::vec3 forwardVec = glm::toMat3(pBoidTransform->getQOrientation()) * FORWARD;
	size_t totalBoids = this->mBoidsInFlock.size();


	// Calculate local flock alignment
	size_t numNeighbours = 0;
	this->mAlignment = forwardVec;
	for (size_t index = 0; index < totalBoids; ++index) {

		if (glm::distance(this->mBoidsInFlock[index].transform->getPosition(), pBoidTransform->getPosition()) < FLOCK_RADIUS) {

			numNeighbours++;
			alignment += glm::toMat3(pBoidTransform->getQOrientation()) * FORWARD;

		}
	}

	// Make sure the alignment vector is greater than zero to avoid divide by zero
	if (glm::length(alignment) != 0.0f) {

		alignment = glm::normalize(this->mAlignment / static_cast<float>(numNeighbours + 1)) * this->alignmentWeight;
	}

	return;
}


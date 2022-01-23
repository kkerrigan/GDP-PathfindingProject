/**
 * @file cBehaviourSystem.cpp
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Implementation of the cBehaviourSystem class.
 */

#include <iostream>

#include "cBehaviourSystem.h"
#include "Common.h"
#include "cBehaviourComponent.h"
#include "cFlockBehaviour.h"

cBehaviourSystem* cBehaviourSystem::getInstance() {

	static cBehaviourSystem instance;

	return &instance;
}


void cBehaviourSystem::update(double deltaTime) {

	// Individual behaviour update
	for (size_t i = 0; i < this->mEntities.size(); ++i) {
		
		cBehaviourComponent* component = this->mEntities[i]->getComponent<cBehaviourComponent>();

		std::vector<iBehaviour*> behaviours = component->getBehaviours();

		for (size_t j = 0; j < behaviours.size(); ++j) {
			
			behaviours[j]->update(this->mEntities[i], deltaTime);
		}
	}

	//update positions after flocking direction determined
	for (size_t index = 0; index < this->mEntities.size(); ++index) {

		cBehaviourComponent* behaviourComp = this->mEntities[index]->getComponent<cBehaviourComponent>();

		if (behaviourComp->hasBehaviour<cFlockBehaviour>()) {

			cFlockBehaviour* flock = behaviourComp->getBehaviour<cFlockBehaviour>();

			// Skip current behaviour if it set to stop
			if (!flock->isStarted()) continue;

			cTransformComponent* boidTransform = this->mEntities[index]->getComponent<cTransformComponent>();
			glm::vec3 flockDirection = flock->flockDirection;

			// Now move the local flocks in their direction
			glm::quat flockOrientation = glm::quat(glm::inverse(
				glm::lookAt(boidTransform->getPosition(), boidTransform->getPosition() - flockDirection, UP)
			));
			boidTransform->setQOrientation(flockOrientation);

			glm::vec3 newFlockPos = boidTransform->getPosition();
			newFlockPos += flockDirection * 10.0f * static_cast<float>(deltaTime);
			boidTransform->setPosition(newFlockPos);
		}
	}

	// Crowd behaviour (formations) update
	for (size_t index = 0; index < this->mCrowdBehaviours.size(); ++index) {

		// Skip this crowd behaviour if it is stopped
		if (!this->mCrowdBehaviours[index]->isStarted()) continue;

		this->mCrowdBehaviours[index]->update();
	}

	return;
}


void cBehaviourSystem::registerEntity(cEntity* entity) {

	this->mEntities.push_back(entity);

	return;
}


void cBehaviourSystem::unregisterEntity(cEntity* entity) {

	// TODO: unregister entity from Behaviour system
	return;
}

std::vector<cEntity*> cBehaviourSystem::getEntities() const {

	return this->mEntities;
}

std::vector<iCrowdBehaviour*> cBehaviourSystem::getCrowdBehaviours() const{

	return this->mCrowdBehaviours;
}

void cBehaviourSystem::registerCrowdBehaviour(iCrowdBehaviour* behaviour) {

	this->mCrowdBehaviours.push_back(behaviour);

	return;
}

void cBehaviourSystem::unregisterCrowdBehaviour(iCrowdBehaviour* behaviour) {

	// TODO: unregister group behaviour

	return;
}
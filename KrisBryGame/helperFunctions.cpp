/**
 * @file helperFunctions.cpp
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 *
 * @section DESCRIPTION
 *
 * This file contains various helper functions for implementing game logic.
 */

#include <vector>

#include <cSceneManager.h>
#include <cBehaviourComponent.h>
#include <cBehaviourSystem.h>
#include <cFlockBehaviour.h>
#include <cCrowdFollowBehaviour.h>

void startFlockBoids(bool toggle) {

	std::vector<cEntity*> entities = cSceneManager::getInstance()->getActiveScene()->getEntities();

	for (size_t index = 0; index < entities.size(); ++index) {

		cEntity* currEntity = entities[index];

		if (currEntity->hasComponent<cBehaviourComponent>()) {

			cBehaviourComponent* behaviour = currEntity->getComponent<cBehaviourComponent>();

			if (behaviour->hasBehaviour<cFlockBehaviour>()) {

				cFlockBehaviour* flock = behaviour->getBehaviour<cFlockBehaviour>();

				if (toggle) {

					flock->startBehaviour();
				}
				else {

					flock->stopBehaviour();
				}
			}
		}
	}

	return;
}

void checkFlockFollowPath() {

	std::vector<cEntity*> entities = cSceneManager::getInstance()->getActiveScene()->getEntities();
	std::vector<iCrowdBehaviour*> crowdBehaviours = cBehaviourSystem::getInstance()->getCrowdBehaviours();
	cCrowdFollowBehaviour* followBehaviour = nullptr;

	// Gret the Crowd Follow behaviour from the Behaviour system (super HACKY)
	for (size_t index = 0; index < crowdBehaviours.size(); ++index) {

		cCrowdFollowBehaviour* crowdFollow = static_cast<cCrowdFollowBehaviour*>(crowdBehaviours[index]);

		if (crowdFollow != nullptr) {

			followBehaviour = crowdFollow;
			break;
		}
	}

	for (size_t index = 0; index < entities.size(); ++index) {

		cEntity* currEntity = entities[index];
		cTransformComponent* currTrans = currEntity->getComponent<cTransformComponent>();

		if (currEntity->hasComponent<cBehaviourComponent>()) {

			cBehaviourComponent* behaviourComp = currEntity->getComponent<cBehaviourComponent>();

			// Grab the Flock behaviours, and update each boid in the flocks
			// flock center to the current sPathNode (this will cause the flock to gradually move along the path
			sPath path = followBehaviour->getPath();
			int currentPathIndex = followBehaviour->getCurrentCheckpoint();
			sPathNode currentObjective = path.pathNodes[currentPathIndex];

			if (behaviourComp->hasBehaviour<cFlockBehaviour>()) {

				cFlockBehaviour* flock = behaviourComp->getBehaviour<cFlockBehaviour>();

				if (!flock->isStarted()) break;

				flock->flockCenter = currentObjective.position;

				float distToCurrentCheckpoint = glm::distance(currentObjective.position, currTrans->getPosition());
				if (distToCurrentCheckpoint <= 1.0f) {

					currentPathIndex++;
					currentObjective = path.pathNodes[currentPathIndex];
					flock->flockCenter = currentObjective.position;
					break;

				}
			}
		}
	}

	return;
}
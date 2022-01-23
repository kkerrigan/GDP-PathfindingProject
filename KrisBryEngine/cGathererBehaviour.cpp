/**
 * @file cGathererBehaviour.cpp
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Implementation of the cGathererBehaviour class.
 */

#include "cGathererBehaviour.h"
#include "cScene.h"
#include "PathingStructs.h"
#include "cPathFindingBehaviour.h"
#include "cIdleBehaviour.h"

cGathererBehaviour::cGathererBehaviour() {

	this->mElapsedStateTime = 0.0f;
	this->mHomeBaseWaitTime = 0.0f;
	this->mResourceWaitTime = 0.0f;
	this->mScene = nullptr;
	this->mCurrentState = eGatherStates::BASE_IDLE;

	return;
}

cGathererBehaviour::~cGathererBehaviour() {

	return;
}

/**
* update
* @brief One of the iBehaviour interface methods, updates the entity every frame.
* @param cEntity* pEntity - The entity to be updated.
* @param float deltaTime - Elapsed time this frame.
* @return void.
*/
void cGathererBehaviour::update(cEntity* pEntity, float deltaTime) {

	// Update the time since the last state change
	this->mElapsedStateTime += deltaTime;

	cTransformComponent* transComp = pEntity->getComponent<cTransformComponent>();

	// Currently waiting at a home base
	cIdleBehaviour* idleBehaviour = static_cast<cIdleBehaviour*>(this->mActiveBehaviour);
	if (idleBehaviour && this->mCurrentState == eGatherStates::BASE_IDLE) {

		idleBehaviour->update(nullptr, deltaTime); // No specific entity needed...just conforms to interface

		// Check if done idling, if so change the state
		if (idleBehaviour->isFinished()) {

			this->mCurrentState = eGatherStates::FIND_RESOURCE;
			this->setActiveBehaviour(nullptr);
			return;
		}
	}

	// eGatherStates::FIND_RESOURCE
	if (this->mCurrentState == eGatherStates::FIND_RESOURCE) {

		sPathFindingNode* goalNode = this->mScene->pathfinding->findClosestTileType(transComp->getPosition(), cMapLoader::eTileType::RESOURCE);
		sPathFinding foundPath = this->mScene->pathfinding->findShortestDijkstraPath(transComp->getPosition(), this->mScene->pathfinding->findClosestNode(transComp->getPosition()), goalNode);

		// Change the state to move to resource
		this->mCurrentState = eGatherStates::MOVE_TO_RESOURCE;
		if (!foundPath.pathPositions.empty()) {

			cPathFindingBehaviour* followPath = new cPathFindingBehaviour();
			followPath->setPath(foundPath);
			this->setActiveBehaviour(followPath);

			return;
		}
	}

	// eGatherStates::MOVE_TO_RESOURCE
	cPathFindingBehaviour* followPathBehaviour = static_cast<cPathFindingBehaviour*>(this->mActiveBehaviour);
	if (followPathBehaviour && this->mCurrentState == eGatherStates::MOVE_TO_RESOURCE) {

		//if the node is in use (someone got there before us and gets the resource)
		if (this->mScene->pathfinding->findClosestNode(followPathBehaviour->getLastNode())->bInUse) {

			// Change the state to find a different resource
			this->mCurrentState = eGatherStates::FIND_RESOURCE;
			this->setActiveBehaviour(nullptr);

			return;
		}

		followPathBehaviour->update(pEntity, deltaTime);
		if (followPathBehaviour->isFinished()) {

			// Change the state to idle at the resource
			this->mCurrentState = eGatherStates::RESOURCE_IDLE;
			this->setActiveBehaviour(new cIdleBehaviour(this->mResourceWaitTime));

			// Mark the node as in use
			this->mScene->pathfinding->findClosestNode(transComp->getPosition())->bInUse = true;
		}
	}

	// eGatherStates::RESOURCE_IDLE
	cIdleBehaviour* idleResourceBehaviour = static_cast<cIdleBehaviour*>(this->mActiveBehaviour);
	if (idleResourceBehaviour && this->mCurrentState == eGatherStates::RESOURCE_IDLE) {

		idleResourceBehaviour->update(nullptr, deltaTime);

		if (idleResourceBehaviour->isFinished()) {

			// Change the state to find the home base
			this->mCurrentState = eGatherStates::FIND_HOME_BASE;
			this->setActiveBehaviour(nullptr);

			this->mScene->pathfinding->findClosestNode(transComp->getPosition())->bIsResource = false;
			return;
		}
	}

	// eGatherStates::FIND_HOME_BASE
	if (this->mCurrentState == eGatherStates::FIND_HOME_BASE) {

		this->setActiveBehaviour(nullptr);

		sPathFindingNode* goalNode = this->mScene->pathfinding->findClosestTileType(transComp->getPosition(), cMapLoader::eTileType::HOME_BASE);
		sPathFinding homePath = this->mScene->pathfinding->findShortestAStarPath(transComp->getPosition(), this->mScene->pathfinding->findClosestNode(transComp->getPosition()), goalNode);

		// Change the state to travelling to home
		this->mCurrentState = eGatherStates::TRAVEL_TO_HOME;

		cPathFindingBehaviour* followHomePath = new cPathFindingBehaviour();
		followHomePath->setPath(homePath);
		this->setActiveBehaviour(followHomePath);
		
		return;
	}

	// eGatherStates::TRAVEL_TO_HOME
	cPathFindingBehaviour* homeFollowPath = static_cast<cPathFindingBehaviour*>(this->mActiveBehaviour);
	if (homeFollowPath && this->mCurrentState == eGatherStates::TRAVEL_TO_HOME) {

		homeFollowPath->update(pEntity, deltaTime);
		if (homeFollowPath->isFinished()) {

			// Change the state to idle at base
			this->mCurrentState = eGatherStates::BASE_IDLE;
			this->setActiveBehaviour(new cIdleBehaviour(this->mHomeBaseWaitTime));
		}
	}

	return;
}

/**
* isFinished
* @brief One of the iBehaviour interface methods, checks to see if the behaviour is finished.
* @return A bool representing whether the behaviour has finished.
*/
bool cGathererBehaviour::isFinished() {
	
	
	return false;
}

/**
* FunctionName
* @brief Add a brief description
* @param Add any params
* @return Describe the return type
*/
void cGathererBehaviour::startBehaviour() {


	return;
}

/**
* FunctionName
* @brief Add a brief description
* @param Add any params
* @return Describe the return type
*/
void cGathererBehaviour::stopBehaviour() {


	return;
}

/**
* FunctionName
* @brief Add a brief description
* @param Add any params
* @return Describe the return type
*/
bool cGathererBehaviour::isStarted() {
	
	
	return false;
}

/**
* setHomeBaseWaitTime
* @brief Sets the time to idle at a Home Base
* @param float waitTime - The amount of idle time.
* @return void.
*/
void cGathererBehaviour::setHomeBaseWaitTime(float waitTime) {

	this->mHomeBaseWaitTime = waitTime;

	return;
}

/**
* setResourceWaitTime
* @brief Sets the amount of time to idle at a resource.
* @param float waitTime - The amount of idle time.
* @return void.
*/
void cGathererBehaviour::setResourceWaitTime(float waitTime) {

	this->mResourceWaitTime = waitTime;

	return;
}

/**
* setActiveScene
* @brief Sets the active scene to have access to the path information.
* @param cScene* scene - The current scene.
* @return void.
*/
void cGathererBehaviour::setActiveScene(cScene* scene) {

	this->mScene = scene;

	return;
}

/**
* FunctionName
* @brief Add a brief description
* @param Add any params
* @return Describe the return type
*/
void cGathererBehaviour::setActiveBehaviour(iBehaviour* behaviour) {

	if (this->mActiveBehaviour != nullptr) {

		delete this->mActiveBehaviour;
	}

	this->mActiveBehaviour = behaviour;

	return;
}

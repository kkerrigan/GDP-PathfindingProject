/**
 * @file cDijkstraFollowPathBehaviour.cpp
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Implementation of the cPathFindingBehaviour class.
 */

#include "cPathFindingBehaviour.h"
#include "cTransformComponent.h"
#include "Common.h"

cPathFindingBehaviour::cPathFindingBehaviour() {

	this->mCurrentNode = 0;
	this->mDirection = 1;
	this->mIsFinished = false;

	return;
}

cPathFindingBehaviour::~cPathFindingBehaviour() {

	return;
}

/**
* update
* @brief One of the iBehaviour interface methods, updates the entity every frame.
* @param cEntity* pEntity - The entity to be updated.
* @param float deltaTime - Elapsed time this frame.
* @return void.
*/
void cPathFindingBehaviour::update(cEntity* pEntity, float deltaTime) {

	// Check to see if the behaviour has finished.
	if (this->mIsFinished) return;

	cTransformComponent* transComp = pEntity->getComponent<cTransformComponent>();

	// Check if we are close to a node on the path
	glm::vec3 entityPos = transComp->getPosition();
	glm::vec3 currNodePos = this->mNodes[this->mCurrentNode];
	float distance = glm::abs(glm::distance(entityPos, currNodePos));

	// If we are close to this node, go to the next one, if possible.
	if (distance <= 0.05f) {
		
		// Goal node (reverse direction)
		size_t goalNode = this->mNodes.size() - 1;
		if (this->mNodes[this->mCurrentNode] == this->mNodes[goalNode]) {

			// We have reached the end of the path, stop here
			this->mIsFinished = true;
			return;
		}

		// TODO: AI Project 3 - Don't overrun this vector
		this->mCurrentNode = (this->mCurrentNode + this->mDirection);
	}

	currNodePos = this->mNodes[this->mCurrentNode];

	// Orient the entity to face the current node
	glm::vec3 direction = glm::normalize(currNodePos - entityPos);
	glm::quat newOrientation = glm::quat(glm::inverse(glm::lookAt(entityPos, entityPos - direction, UP)));

	// If close, start the turning
	distance = glm::distance(entityPos, currNodePos);

	if (distance >= 3.0f) {
		
		newOrientation = glm::mix(transComp->getQOrientation(), newOrientation, 0.05f);
	}
	else {
		
		newOrientation = newOrientation;
	}

	transComp->setQOrientation(newOrientation);

	glm::vec3 forwardVec = glm::toMat3(transComp->getQOrientation()) * FORWARD;

	transComp->setPosition(entityPos + forwardVec * 0.025f);

	return;
}

/**
* isFinished
* @brief One of the iBehaviour interface methods, checks to see if the behaviour is finished.
* @return A bool representing whether the behaviour has finished.
*/
bool cPathFindingBehaviour::isFinished() {
	
	return this->mIsFinished;
}

/**
* FunctionName
* @brief Add a brief description
* @param Add any params
* @return Describe the return type
*/
void cPathFindingBehaviour::startBehaviour() {



	return;
}

/**
* FunctionName
* @brief Add a brief description
* @param Add any params
* @return Describe the return type
*/
void cPathFindingBehaviour::stopBehaviour() {



	return;
}

/**
* FunctionName
* @brief Add a brief description
* @param Add any params
* @return Describe the return type
*/
bool cPathFindingBehaviour::isStarted() {
	
	
	return false;
}

/**
* getLastNode
* @brief Gets the last node of the path.
* @return A glm::vec3 representing the last node of the path.
*/

glm::vec3 cPathFindingBehaviour::getLastNode() {
	
	return this->mNodes.back();
}

/**
* setPath
* @brief Sets the path that the entity will follow.
* @param std::vector<glm::vec3> path - The path to be set.
* @return void.
*/
void cPathFindingBehaviour::setPath(std::vector<glm::vec3> path) {

	this->mNodes = path;

	return;
}

/**
* setPath
* @brief Sets the path that the entity will follow using a sDijkstraPath.
* @param sDijkstraPath path - The path to be set.
* @return void.
*/
void cPathFindingBehaviour::setPath(sPathFinding path) {

	this->mNodes = path.pathPositions;

	return;
}

/**
 * @file cFollowBehaviour.cpp
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Implementation of the cFollowBehaviour class.
 */

#include "cFollowBehaviour.h"
#include "Common.h"
#include "cTransformComponent.h"

cFollowBehaviour::cFollowBehaviour() {

	this->mCurrentNode = 0;
	this->mDirection = 1;

	return;
}

cFollowBehaviour::~cFollowBehaviour() {

	return;
}

void cFollowBehaviour::addCheckpoint(sPathNode checkpoint, int index) {

	// Check if a position was passed int for new checkpoint
	if (index == -1) {

		this->mPath.pathNodes.push_back(checkpoint);
		return;
	}

	// Otherwise place the new checkpoint at the specified index
	std::vector<sPathNode>::iterator nodeIter = this->mPath.pathNodes.begin() + index;
	this->mPath.pathNodes.insert(nodeIter, 1, checkpoint);
	
	return;

}

void cFollowBehaviour::setPath(sPath checkpoints) {

	this->mPath = checkpoints;

	return;
}

sPath& cFollowBehaviour::getPath() {
	
	return this->mPath;
}

void cFollowBehaviour::update(cEntity* pEntity, float deltaTime) {

	cTransformComponent* pTransComp = pEntity->getComponent<cTransformComponent>();

	// Check if we are at a checkpoint node
	glm::vec3 entityPos = pTransComp->getPosition();
	sPathNode currCheckpoint = this->mPath.pathNodes[this->mCurrentNode];
	float distanceToCheckpoint = glm::distance(entityPos, currCheckpoint.position);

	// If we are at the checkpoint, change to the next one (if possible)
	if (distanceToCheckpoint <= 0.5f) {

		std::vector<sPathNode>::iterator nodeIter = this->mPath.pathNodes.begin() + this->mCurrentNode;
		if (nodeIter != this->mPath.pathNodes.end()) {

			this->mCurrentNode += 1;
		}
	}

	currCheckpoint = this->mPath.pathNodes[this->mCurrentNode];

	// Face the entity towards the current checkpoint node
	glm::vec3 direction = glm::normalize(currCheckpoint.position - entityPos);
	glm::quat entityOri = glm::quat(glm::inverse(glm::lookAt(entityPos, entityPos - direction, UP)));

	// If we are close to current checkpoint, start turning towards the next one
	distanceToCheckpoint = glm::distance(entityPos, currCheckpoint.position);
	if (distanceToCheckpoint >= 3.0f) { 

		entityOri = glm::mix(pTransComp->getQOrientation(), entityOri, 0.1f);
	}

	// Set the new orientation
	pTransComp->setQOrientation(entityOri);

	// Set the new position
	glm::vec3 forwardVec = glm::toMat3(pTransComp->getQOrientation()) * FORWARD;
	pTransComp->setPosition(entityPos + forwardVec * 0.5f);

	return;
}

bool cFollowBehaviour::isFinished() {
	
	// Not implemented on this behaviour
	
	return false;
}

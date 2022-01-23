/**
 * @file cCrowdFollowBehaviour.cpp
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 *
 * @section DESCRIPTION
 *
 * Implementation of cCrowdFollowComponent class.
 */

#include "cCrowdFollowBehaviour.h"
#include "Common.h"
#include "cTransformComponent.h"

cCrowdFollowBehaviour::cCrowdFollowBehaviour(cFormationBehaviour* formation) {

	this->mCrowdFormation = formation;
	this->mCurrentNode = 0;
	this->mDirection = 1;
	this->mBIsFollowingPath = false;
	this->mBIsBehaviourStarted = false;

	return;
}

cCrowdFollowBehaviour::~cCrowdFollowBehaviour() {

	return;
}

void cCrowdFollowBehaviour::registerEntity(cEntity* pEntity) {

	// Not implemented for this behaviour

	return;
}

void cCrowdFollowBehaviour::unregisterEntity(cEntity* pEntity) {

	// Not implemented on this behaviour

	return;
}

void cCrowdFollowBehaviour::update() {

	// Check if the crowd hasn't been told to follow the path
	if (!this->mBIsFollowingPath) {

		this->mCrowdFormation->update();
		return;
	}

	glm::vec3 crowdNucleus = this->mCrowdFormation->getCrowdNucleus();
	sPathNode currCheckpoint = this->mCheckpoints.pathNodes[this->mCurrentNode].position;
	float distToCurrentCheckpoint = glm::distance(crowdNucleus, currCheckpoint.position);

	// If we have reached our current checkpoint, move to the next one (if available)
	if (distToCurrentCheckpoint <= 1.0f) {

		if (this->mDirection == 1) { // Following path in the forward direction

			this->mCurrentNode = (this->mCurrentNode + 1) % this->mCheckpoints.pathNodes.size();
		}
		else {

			this->mCurrentNode = (this->mCurrentNode - 1);

			if (this->mCurrentNode < 0) {

				this->mCurrentNode = this->mCheckpoints.pathNodes.size() - 1;
			}
		}
	}


	currCheckpoint = this->mCheckpoints.pathNodes[this->mCurrentNode];

	// Orient the entities to the current checkpoint
	glm::vec3 direction = glm::normalize(currCheckpoint.position - crowdNucleus);
	glm::quat checkpointOrientation = glm::quat(glm::inverse(
		glm::lookAt(crowdNucleus, crowdNucleus - direction, UP)));

	// Set the new values for the formation
	glm::vec3 forwardVec = glm::toMat3(checkpointOrientation) * FORWARD;
	this->mCrowdFormation->setCrowdNucleus(crowdNucleus + forwardVec * 0.25f);
	this->mCrowdFormation->setCrowdDirection(direction);

	this->mCrowdFormation->update();

	return;
}

void cCrowdFollowBehaviour::startBehaviour() {

	this->mBIsBehaviourStarted = true;

	return;
}

void cCrowdFollowBehaviour::stopBehaviour() {

	this->mBIsBehaviourStarted = false;

	return;
}

bool cCrowdFollowBehaviour::isStarted() {
	
	return this->mBIsBehaviourStarted;
}

void cCrowdFollowBehaviour::addCheckpoint(sPathNode checkpoint, int index) {

	// Check to see if no index was passed in, if not place new checkpoint at end
	if (index == -1) {

		this->mCheckpoints.pathNodes.push_back(checkpoint);
		return;
	}

	// Otherwise place the new checkpoint at the given index
	std::vector<sPathNode>::iterator iter = this->mCheckpoints.pathNodes.begin() + index;
	this->mCheckpoints.pathNodes.insert(iter, 1, checkpoint);

	return;
}

void cCrowdFollowBehaviour::setPath(sPath checkpoints) {

	this->mCheckpoints = checkpoints;

	return;
}

int cCrowdFollowBehaviour::getCurrentCheckpoint() {
	
	return this->mCurrentNode;
}

sPath& cCrowdFollowBehaviour::getPath() {

	return this->mCheckpoints;
}

void cCrowdFollowBehaviour::toggleDirection() {

	if (this->mDirection == 1) {

		this->mCurrentNode = this->mCurrentNode - 1;

		// Set the current node to the beginning of the flock
		if (this->mCurrentNode < 0) {

			this->mCurrentNode = 0;
		}
		// Set to -1 for backwards
		this->mDirection = -1;
	}
	else {

		this->mCurrentNode = (this->mCurrentNode + 1) % this->mCheckpoints.pathNodes.size();

		// Set back to 1 for forward
		this->mDirection = 1;
	}

	return;
}

void cCrowdFollowBehaviour::togglePathFollowing() {

	this->mBIsFollowingPath = !this->mBIsFollowingPath;

	return;
}

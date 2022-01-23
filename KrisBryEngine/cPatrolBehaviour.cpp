/**
 * @file cPatrolBehaviour.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Implementation of the cPatrolBehaviour class.
 */

#include "Global.h"

#include "cPatrolBehaviour.h"

cPatrolBehaviour::cPatrolBehaviour(cEntity* agent, float timeBetweenWander, float mapExtents) {

	this->mAgent = agent;
	this->mTimeBetweenWanders = timeBetweenWander;
	this->mMapExtents = mapExtents;
	
	this->mWander = new cWanderBehaviour(this->mAgent, this->mMapExtents);
	this->mIdle = new cIdleBehaviour(this->mTimeBetweenWanders);
	this->mIsIdle = true;

	return;
}

cPatrolBehaviour::~cPatrolBehaviour() {

	return;
}

void cPatrolBehaviour::update(cEntity* pEntity, float deltaTime) {

	// check if the current behaviour is wander and if its finished
	if (this->mIsIdle) {

		if (mIdle->isFinished()) {

			this->mIsIdle = false;
			this->mWander->findNewPosition(this->mMapExtents);
			this->mWander->update(this->mAgent, deltaTime);
		}
		else {
			this->mIdle->update(this->mAgent, deltaTime);
		}

	}
	else {
		if (this->mWander->isFinished()) {

			this->mIsIdle = true;
			this->mIdle->refreshIdle();
		}
		else {
			this->mWander->update(this->mAgent, deltaTime);
		}
	}
	
	return;
}

bool cPatrolBehaviour::isFinished() {
	
	return false;
}

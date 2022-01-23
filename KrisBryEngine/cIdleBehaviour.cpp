/**
 * @file cIdleBehaviour.cpp
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Implementation of the cIdleBehaviour class.
 */

#include "cIdleBehaviour.h"

cIdleBehaviour::cIdleBehaviour(float timeToWait) {

	this->mTimeToWait = timeToWait;
	this->mTotalTime = 0.0f;

	return;
}

cIdleBehaviour::~cIdleBehaviour() {

	return;
}

/**
* update
* @brief One of the iBehaviour interface methods, updates the entity every frame.
* @param cEntity* pEntity - The entity to be updated.
* @param float deltaTime - Elapsed time this frame.
* @return void.
*/
void cIdleBehaviour::update(cEntity* pEntity, float deltaTime) {

	// Check if the behaviour is finished
	if (this->isFinished()) return;

	// Otherwise add to the total time 
	this->mTotalTime += deltaTime;

	return;
}

/**
* isFinished
* @brief One of the iBehaviour interface methods, checks to see if the behaviour is finished.
* @return A bool representing whether the behaviour has finished.
*/
bool cIdleBehaviour::isFinished() {
	
	return this->mTotalTime >= this->mTimeToWait;
}

/**
* FunctionName
* @brief Add a brief description
* @param Add any params
* @return Describe the return type
*/
void cIdleBehaviour::startBehaviour() {

	return;
}

/**
* FunctionName
* @brief Add a brief description
* @param Add any params
* @return Describe the return type
*/
void cIdleBehaviour::stopBehaviour() {


	return;
}

/**
* FunctionName
* @brief Add a brief description
* @param Add any params
* @return Describe the return type
*/
bool cIdleBehaviour::isStarted() {
	
	return false;
}

/**
* refreshIdle
* @brief Refreshes the wait time on the behaviour.
* @return void.
*/
void cIdleBehaviour::refreshIdle() {

	this->mTotalTime = 0.0f;

	return;
}

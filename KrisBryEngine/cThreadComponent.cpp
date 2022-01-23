/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

// Gems Midterm Q3

#include <iostream>
#include <algorithm>
#include <cfloat>

#include "Global.h"
#include "cThreadComponent.h"
#include "cRandThreaded.h"
#include "cTransformComponent.h"
#include "cSceneManager.h"

DWORD WINAPI threadManager(void* pInitialData) {

	// Deref the this pointer to get at the specific Dalek
	cEntity* pEntity = (cEntity*)pInitialData;

	// Delay the start for about 250 ms
	Sleep(250);

	// Now endlessly run the loop until it's time to exit
	while (true) {
		pEntity->getComponent<cThreadComponent>()->update();

		// Release control of the thread
		Sleep(1);

	}

	return 0;
}


cThreadComponent::cThreadComponent() : cComponent() {

	this->bIsAlive = true;

	this->m_Velocity = glm::vec3(0.0f, 0.0f, 0.0f);

	this->m_pLocalTimer = new CHRTimer();
	this->m_pLocalTimer->ResetAndStart();

	this->m_TimeToMove = 0.0f;

	this->m_pRandThread = new cRandThreaded();

	return;
}

void cThreadComponent::createThreadAndStartUpdating(cEntity* pEntity) {

	this->pEntity = pEntity;

	LPDWORD phThread = 0;	// Clear to zero
	DWORD hThread = 0;
	HANDLE hThreadHandle = 0;

	// Pass a pointer to this instance 
	// Recal that the "this" pointer is the pointer to
	//	this particular instance of the object
	void* pThisEntity = (void*)(pEntity);

	std::cout << "Creating Dalek thread..." << std::endl;
	std::cout.flush();

	this->pickNewDirection();

	hThreadHandle = CreateThread(NULL,	// Default security
		0,								// Stack size - default - win32 = 1 Mbyte
		&threadManager,					// Pointer to the thread's function
		pThisEntity,					// The value (parameter) we pass to the thread
		0,								// CREATE_SUSPENDED or 0 for threads...
		(DWORD*)&phThread);				// pointer or ref to variable that will get loaded with threadID


	return;
}

void cThreadComponent::update() {

	float deltaTime = this->m_pLocalTimer->GetElapsedSeconds(true);

	glm::vec3 deltaVel = this->m_Velocity * deltaTime;

	cTransformComponent* transComp = this->pEntity->getComponent<cTransformComponent>();

	glm::vec3 pos = transComp->getPosition();
	pos += deltaVel;

	transComp->setPosition(pos);


	if (glm::distance(pos, this->mTarget) <= 0.1f) {
		std::vector<glm::vec3> goodSpots = cSceneManager::getInstance()->getActiveScene()->emptySquares;

		int closestIndex = 0;
		float closestDistance = FLT_MAX;
		for (int i = 0; i < goodSpots.size(); ++i) {
			
			float dist = glm::distance(pos, goodSpots[i]);
			
			if(dist < closestDistance) {
				closestIndex = i;
				closestDistance = dist;
			}
		}

		transComp->setPosition(goodSpots[closestIndex]);

		this->pickNewDirection();
	}


	return;
}

void cThreadComponent::pickNewDirection() {	

	int direction = getRandInRange(1, 4);

	cTransformComponent* dalekTransComp = this->pEntity->getComponent<cTransformComponent>();
	glm::vec3 position = dalekTransComp->getPosition();

	// Call increment position with the random number
	position = this->m_IncrementPosition(position, direction);

	// Check to see if its a valid direction
	int possibleMoves = this->m_CheckDirection(position, direction);
	while (possibleMoves == 0) {

		direction = getRandInRange(1, 4);
		position = dalekTransComp->getPosition();
		this->m_IncrementPosition(position, direction);
		possibleMoves = this->m_CheckDirection(position, direction);
	}

	int numMoves = getRandInRange(1, possibleMoves);

	switch (direction) {
	case 1:
		// Up (+Z)
		position.z += (4 * numMoves);
		break;
	case 2:
		// Down (-Z)
		position.z -= (4 * numMoves);
		break;
	case 3:
		// Right (+X)
		position.x += (4 * numMoves);
		break;
	case 4:
		// Left (-X)
		position.x -= (4 * numMoves);
		break;
	}

	this->mTarget = position;
	this->m_Velocity = glm::normalize(this->mTarget - dalekTransComp->getPosition())  * 3.0f;


	return;
}

int cThreadComponent::m_CheckDirection(glm::vec3 position, int direction, int count) {

	std::vector<glm::vec3> positions = cSceneManager::getInstance()->getActiveScene()->emptySquares;

	std::vector<glm::vec3>::iterator it = std::find(positions.begin(), positions.end(), position);

	if (it == positions.end()) {

		return count;
	}
	else {

		count++;
		this->m_IncrementPosition(position, direction);
		return this->m_CheckDirection(position, direction, count);
	}

	
}

glm::vec3 cThreadComponent::m_IncrementPosition(glm::vec3& currentPosition, int direction) {

	switch (direction) {
	case 1:
		// Up (+Z)
		currentPosition.z += 4;
		return currentPosition;
		break;
	case 2:
		// Down (-Z)
		currentPosition.z -= 4;
		return currentPosition;
		break;
	case 3:
		// Right (+X)
		currentPosition.x += 4;
		return currentPosition;
		break;
	case 4:
		// Left (-X)
		currentPosition.x -= 4;
		return currentPosition;
		break;
	}

}

void cThreadComponent::resetTimerAndStart() {

	this->m_pLocalTimer->ResetAndStart();

	return;
}
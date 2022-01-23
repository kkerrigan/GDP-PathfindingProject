#include "cFireCannons.h"
#include "cEntityBuilder.h"
#include "cTransformComponent.h"
#include "cRenderMeshComponent.h"
#include "cEulerMovementComponent.h"
#include <iostream>

// Forward declare this function defined in loadTerrainAABBHierarchy.cpp
glm::vec3 converVertsToWorld(glm::vec3 point, glm::vec3 position, glm::quat orientation);

cFireCannons::cFireCannons(cEntity * pAttacker, cEntity * pTarget) {

	this->pAttackingShip = pAttacker;
	this->pTargetShip = pTarget;
	this->bIsFinished = false;

	// Get the attacking ship position to render cannon balls
	cTransformComponent* pAttackTrans = this->pAttackingShip->getComponent<cTransformComponent>();
	glm::vec3 cannonBallOffset = glm::vec3(0.75f, 1.0f, 0.0f);

	// Set up four cannon balls along the ship
	for (unsigned int index = 0; index != 4; ++index) {

		cEntity* pCannonBall = cEntityBuilder::getInstance()->createEntity(0);
		this->pCannonBalls.push_back(pCannonBall);

		cTransformComponent* pCannonTrans = pCannonBall->getComponent<cTransformComponent>();
		cRenderMeshComponent* pCannonMesh = pCannonBall->getComponent<cRenderMeshComponent>();

		pCannonTrans->setUniformScale(0.25f);

		glm::vec3 pos(0.0f);
		// Move the cannon ball down the ship a little each time
		pos.z += (0.75 * index);
		// Convert to world space to account for model orientation and scale
		glm::vec3 worldSpace = converVertsToWorld(pos + cannonBallOffset, pAttackTrans->getPosition(), pAttackTrans->orientation);
		pCannonTrans->setPosition(worldSpace);

		pCannonMesh->bIsVisible = true;
		pCannonMesh->bIsWireFrame = true;

		if (this->pAttackingShip->UNIQUE_ID == 6) {
			pCannonMesh->setDiffuseColour(glm::vec3(0.0f, 1.0f, 0.0f));
		}

		if (this->pAttackingShip->UNIQUE_ID == 7) {
			pCannonMesh->setDiffuseColour(glm::vec3(1.0f, 0.0f, 0.0f));
		}
	}

	return;
}

cFireCannons::~cFireCannons() {}

void cFireCannons::update(float deltaTime) {

	this->reloadTime += deltaTime;

	float nextShot = this->getRandInRange(2.0f, 6.0f);

	if (this->reloadTime > nextShot) {
		cEntity* currentCannon = this->chooseRandomBullet();

		if (currentCannon == nullptr) return;

		cTransformComponent* pTransComp = currentCannon->getComponent<cTransformComponent>();
		cEulerMovementComponent* pEulerComp = currentCannon->getComponent<cEulerMovementComponent>();

		// Determine direction to the target
		cTransformComponent* pAttackerTrans = this->pAttackingShip->getComponent<cTransformComponent>();
		cTransformComponent* pTargetTrans = this->pTargetShip->getComponent<cTransformComponent>();

		glm::vec3 distance = pAttackerTrans->getPosition() - pTargetTrans->getPosition();
		glm::vec3 direction = glm::normalize(distance);
		// Set the y of the direction to 1.0 so the cannons always fire upwards
		direction.y = 1.0f;

		float randomXVel = this->getRandInRange(-1.0f, 5.0f);
		float randomYVel = this->getRandInRange(20.0f, 35.0f);
		pEulerComp->velocity = glm::vec3(randomXVel, randomYVel, -25.0f) * direction;
		pEulerComp->acceleration = glm::vec3(0.0f, -10.0f, 0.0f);

		reloadTime = 0.0f;
	}

	return;
}

bool cFireCannons::isFinished() {
	return this->bIsFinished;
}

cEntity* cFireCannons::chooseRandomBullet() {
	
	int randomNum = this->getRandInRange(0, (unsigned int)this->pCannonBalls.size());
	
	cEntity* randomCannon = this->pCannonBalls[randomNum];

	// Set this one to a nullptr so I know when the command is finished
	this->pCannonBalls[randomNum] = nullptr;

	return randomCannon;
}

float cFireCannons::getRandInRange(float min, float max) {

	float value =
		min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (static_cast<float>(max - min))));
	return value;
}

unsigned int cFireCannons::getRandInRange(unsigned int min, unsigned int max) {

	// You could do this with the modulus operator (since it's an int)
	float value =
		min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (static_cast<float>(max - min))));
	return (unsigned int)value;
}

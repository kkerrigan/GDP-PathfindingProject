#include "cEulerMovementSystem.h"



cEulerMovementSystem::cEulerMovementSystem(){}

cEulerMovementSystem::~cEulerMovementSystem(){}


cEulerMovementSystem* cEulerMovementSystem::getInstance() {
	static cEulerMovementSystem instance;
	
	return &instance;
}

void cEulerMovementSystem::update(double deltaTime) {
	
	for (cEntity* entity : this->entitiesToUpdate) {

		//get the entities position
		cTransformComponent* position = entity->getComponent<cTransformComponent>(); // static cast good as we know the component is there

		//get the entities euler velocity and acceleration
		cEulerMovementComponent* eulerMovement = entity->getComponent<cEulerMovementComponent>(); // static cast good as we know the component is there

		// acceleration 
		eulerMovement->velocity += eulerMovement->acceleration * (float)deltaTime;
		
		if (eulerMovement->velocity.x > 75.0f) {
			eulerMovement->velocity.x = 75.0f;
		}

		if (eulerMovement->velocity.x < -75.0f) {
			eulerMovement->velocity.x = -75.0f;
		}

		if (eulerMovement->velocity.y > 75.0f) {
			eulerMovement->velocity.y = 75.0f;
		}		

		if (eulerMovement->velocity.y < -75.0f) {
			eulerMovement->velocity.y = -75.0f;
		}

		if (eulerMovement->velocity.z > 75.0f) {
			eulerMovement->velocity.z = 75.0f;
		}

		if (eulerMovement->velocity.z < -75.0f) {
			eulerMovement->velocity.z = -75.0f;
		}

		//movement
		glm::vec3 pos = position->getPosition();
		pos += eulerMovement->velocity * (float)deltaTime;
		position->setPosition(pos);
	}
}

void cEulerMovementSystem::registerEntity(cEntity* entity) {
	this->entitiesToUpdate.push_back(entity);
}

void cEulerMovementSystem::unregisterEntity(cEntity* entity) {
	// TODO: remove from vector
}
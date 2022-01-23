#include "cOrientToEntity.h"
#include "Common.h"
#include "cTransformComponent.h"

cOrientToEntity::cOrientToEntity(cEntity* entityToRotate, cEntity* entityTarget, float duration, float easeIn, float easeOut):
	entityToRotate(entityToRotate),
	entityTarget(entityTarget), firstUpdateDone(false),
	initialTime(0),
	elapsedTime(0), duration(duration),
	finished(false) {
	
	initialPosition = this->entityTarget->getComponent<cTransformComponent>()->getPosition();
	lookAtPosition = initialPosition;
	theUpVector = UP;
	initialOrientation = glm::toMat4(this->entityTarget->getComponent<cTransformComponent>()->orientation);

	this->easeIn = easeIn;
	this->easeOut = easeOut;

	this->finalOrientation = glm::inverse(glm::lookAt(
		this->entityToRotate->getComponent<cTransformComponent>()->getPosition(), this->lookAtPosition,
		this->theUpVector));

	return;
}

void cOrientToEntity::update(float deltaTime) {
	
	if(!this->firstUpdateDone) {
		
		this->initialTime = glfwGetTime();
		this->elapsedTime = 0.0f;

		initialPosition = this->entityTarget->getComponent<cTransformComponent>()->getPosition();
		lookAtPosition = initialPosition;
		theUpVector = UP;
		initialOrientation = glm::toMat4(this->entityToRotate->getComponent<cTransformComponent>()->orientation);

		float distance = glm::distance(this->entityTarget->getComponent<cTransformComponent>()->getPosition(), this->entityToRotate->getComponent<cTransformComponent>()->getPosition());

		glm::vec3 direction = glm::normalize(this->entityTarget->getComponent<cTransformComponent>()->getPosition() - this->entityToRotate->getComponent<cTransformComponent>()->getPosition());

		//rotation
		this->finalOrientation = glm::toMat4(glm::quatLookAt(-direction, UP));
		
		this->firstUpdateDone = true;
	}

	cTransformComponent* entityTransform = this->entityToRotate->getComponent<cTransformComponent>();
	cTransformComponent* targetTransform = this->entityTarget->getComponent<cTransformComponent>();

	glm::quat quatStart = glm::quat_cast(this->initialOrientation);
	glm::quat quatEnd = glm::quat_cast(this->finalOrientation);

	this->elapsedTime = glfwGetTime() - this->initialTime;
	float factor = this->elapsedTime / this->duration;

	//ease in / ease out
	float easeInFactor = 1.0f;
	if (this->easeIn > 0.0f && this->elapsedTime <= this->easeIn) {
		easeInFactor = this->elapsedTime / this->easeIn;
	}

	float easeOutFactor = 1.0f;
	if (this->easeOut > 0.0f && (this->duration - this->easeOut) < this->elapsedTime) {
		easeOutFactor = (this->elapsedTime / this->duration);
	}

	//interpolate the rotation between the start and end rotation
	glm::quat quatInterp = glm::slerp(quatStart, quatEnd, factor * easeInFactor * easeOutFactor);

	// Transform the resulting quaternion back to the Matrix
	this->entityToRotate->getComponent<cTransformComponent>()->orientation = glm::quat(quatInterp);

	if(glm::toMat4(quatInterp) == this->finalOrientation || this->elapsedTime >= this->duration){	
		this->finished = true;
	}

	return;
}

bool cOrientToEntity::isFinished() {
	return this->finished;
}

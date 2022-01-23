#include "cOrientToEntityDistance.h"
#include "Common.h"
#include "Global.h"
#include "cTransformComponent.h"

cOrientToEntityDistance::cOrientToEntityDistance(cEntity* entityToRotate, cEntity* entityTarget, float duration,
	float easeIn, float easeOut) : entityToRotate(entityToRotate),
	entityTarget(entityTarget), firstUpdateDone(false),
	initialTime(0),
	elapsedTime(0), duration(duration),
	finished(false) {
	
	initialPosition = this->entityTarget->getComponent<cTransformComponent>()->getPosition();
	lookAtPosition = initialPosition;
	theUpVector = UP;
	initialOrientation = glm::toMat4(this->entityTarget->getComponent<cTransformComponent>()->orientation);
	
	this->easeInDistance = easeIn;
	this->easeOutDistance = easeOut;

	this->finalOrientation = glm::inverse(glm::lookAt(
		this->entityToRotate->getComponent<cTransformComponent>()->getPosition(), this->lookAtPosition,
		this->theUpVector));

	return;
}

void cOrientToEntityDistance::update(float deltaTime) {
	
	if (!this->firstUpdateDone) {
		
		this->initialTime = glfwGetTime();
		this->elapsedTime = 0.0f;

		initialPosition = this->entityTarget->getComponent<cTransformComponent>()->getPosition();
		lookAtPosition = initialPosition;
		theUpVector = UP;
		initialOrientation = glm::toMat4(this->entityToRotate->getComponent<cTransformComponent>()->orientation);

		this->finalOrientation = glm::inverse(glm::lookAt(
			this->entityToRotate->getComponent<cTransformComponent>()->getPosition(), this->lookAtPosition,
			this->theUpVector));

		this->firstUpdateDone = true;
	}

	cTransformComponent* entityTransform = this->entityToRotate->getComponent<cTransformComponent>();
	cTransformComponent* targetTransform = this->entityTarget->getComponent<cTransformComponent>();


	// Transform the Orientation Matrix to Quaternion
	glm::quat quatStart = glm::quat_cast(this->initialOrientation);
	glm::quat quatEnd = glm::quat_cast(this->finalOrientation);

	this->elapsedTime = glfwGetTime() - this->initialTime;
	float factor = this->elapsedTime / this->duration;

	//ease in / ease out
	//currently slerps so we have smoothing on each end

	// Do the orientation calculation over the Quaternion
	glm::quat quatInterp = glm::slerp(quatStart, quatEnd, factor);

	// Transform the resulting quaternion back to the Matrix
	this->entityToRotate->getComponent<cTransformComponent>()->orientation = glm::quat(quatInterp);

	//are we done?
	if (glm::toMat4(quatInterp) == this->finalOrientation || this->elapsedTime >= this->duration) {
		this->finished = true;
	}

	return;
}

bool cOrientToEntityDistance::isFinished() {
	return this->finished;
}

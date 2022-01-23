#include "cCameraOrientToEntity.h"
#include "Common.h"
#include "cTransformComponent.h"

cCameraOrientToEntity::cCameraOrientToEntity(cFlyCamera* camera, cEntity* entityTarget, float duration, float easeIn, float easeOut): camera(camera),
	entityTarget(entityTarget), firstUpdateDone(false),
	initialTime(0),
	elapsedTime(0), duration(duration),
	finished(false){}

void cCameraOrientToEntity::update(float deltaTime) {
	
	if (!this->firstUpdateDone) {
		
		this->initialTime = glfwGetTime();
		this->elapsedTime = 0.0f;

		initialPosition = this->entityTarget->getComponent<cTransformComponent>()->getPosition();
		lookAtPosition = initialPosition;
		theUpVector = UP;
		initialOrientation = glm::toMat4(this->camera->getQOrientation());
	}

	glm::vec3 direction = glm::normalize(this->entityTarget->getComponent<cTransformComponent>()->getPosition() - this->camera->eye);

	//rotation
	this->finalOrientation = glm::toMat4(glm::quatLookAt(-direction, UP));

	this->firstUpdateDone = true;

	cTransformComponent* targetTransform = this->entityTarget->getComponent<cTransformComponent>();


	// Transform the Orientation Matrix to Quaternion
	glm::quat quatStart = glm::quat_cast(this->initialOrientation);
	glm::quat quatEnd = glm::quat_cast(this->finalOrientation);

	this->elapsedTime = glfwGetTime() - this->initialTime;
	float factor = this->elapsedTime / this->duration;

	if(factor > 1.0f) {
		factor = 1.0f;
	}

	// Transform the resulting quaternion back to the Matrix
	// NOTE: No slerp done in this command because we always want to be snapped to
	// the selected entity.
	this->camera->setMeshOrientationQ(glm::quat(finalOrientation));

	//are we done?
	if (this->elapsedTime >= this->duration) {
		this->finished = true;
	}
}

bool cCameraOrientToEntity::isFinished() {
	return this->finished;
}

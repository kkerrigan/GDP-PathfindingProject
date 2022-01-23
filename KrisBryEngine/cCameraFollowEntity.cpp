#include "cCameraFollowEntity.h"
#include "Common.h"
#include "cScriptUtil.h"
#include "cTransformComponent.h"

cCameraFollowEntity::cCameraFollowEntity(cFlyCamera* camera, cEntity* entityToFollow, glm::vec3 followDistance, float minDistance,
	float maxSpeedDistance, float maxSpeed, glm::vec3 offset, float easeInRange, float easeOutRange, float time) : camera(camera), entityToFollow(entityToFollow),
followDistance(followDistance),
offset(offset), minDistance(minDistance),
maxSpeed(maxSpeed),
maxSpeedDistance(maxSpeedDistance),
easeInRange(easeInRange),
easeOutRange(easeOutRange), orientToTarget(false),
finished(false), time(time), elapsedTime(0.0f) {}


void cCameraFollowEntity::update(float deltaTime) {
	
	this->elapsedTime += deltaTime;

	cTransformComponent* entityToFollowTransform = this->entityToFollow->getComponent<cTransformComponent>();

	// Get the information from the object that is moving
	float currentDistance = glm::distance(entityToFollowTransform->getPosition() + this->offset, this->camera->eye);

	if (this->elapsedTime >= this->time) {
		
		this->finished = true;
		return;
	}

	// Here, I am NOT where I'm supposed to be, so I need to move.
	// Calculate the direction vector to the target location
	glm::vec3 idealCameraLocation = entityToFollowTransform->getPosition() + this->offset;

	// To calc velocity, I will normalize
	glm::vec3 vVel = glm::normalize(idealCameraLocation);

	// Adjust velocity in the "slow down" zone
	currentDistance = glm::distance(idealCameraLocation, this->camera->eye);

	float distance = cScriptUtil::clamp(this->minDistance, this->maxSpeedDistance, currentDistance);

	// Now distance will be MAX this->maxSpeedDistance...
	// Scale this from 0 to 1 (1 at max distance)
	float slowDownRatio = distance / (this->maxSpeedDistance - this->minDistance);

	// Smoothstep
	slowDownRatio = cScriptUtil::smoothstep_wikipedia(0.0f, 1.0f, slowDownRatio);

	//	std::cout << "slowDownRatio = " << slowDownRatio << std::endl;
	glm::vec3 direction = glm::normalize(entityToFollowTransform->getPosition() + this->offset - this->camera->eye);

	if(distance > this->minDistance) {
		
		this->camera->eye += direction * slowDownRatio * this->maxSpeed * deltaTime;
	}
	
	if (this->orientToTarget) {
		
		glm::quat orient = (glm::lookAt(camera->eye - offset, entityToFollowTransform->getPosition(), UP));

		camera->setMeshOrientationQ((orient));
	}

	return;
}

bool cCameraFollowEntity::isFinished() {
	return this->finished;
}

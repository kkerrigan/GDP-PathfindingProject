#include "cFollowEntity.h"
#include "cScriptUtil.h"
#include "cTransformComponent.h"
#include "cEulerMovementComponent.h"

cFollowEntity::cFollowEntity(cEntity* entity, cEntity* entityToFollow, glm::vec3 followDistance, float minDistance,
                             float maxSpeedDistance, float maxSpeed, glm::vec3 offset,
							 float easeInRange, float easeOutRange): entity(entity), entityToFollow(entityToFollow),
							                                         followDistance(followDistance),
							                                         offset(offset), minDistance(minDistance),
							                                         maxSpeed(maxSpeed),
							                                         maxSpeedDistance(maxSpeedDistance),
							                                         easeInRange(easeInRange),
							                                         easeOutRange(easeOutRange), orientToTarget(true),
							                                         finished(false) {}

void cFollowEntity::update(float deltaTime) {
	
	cTransformComponent* entityTransform = this->entity->getComponent<cTransformComponent>();
	cEulerMovementComponent* entityEuler = this->entity->getComponent<cEulerMovementComponent>();

	cTransformComponent* entityToFollowTransform = this->entityToFollow->getComponent<cTransformComponent>();

	// Get the information from the object that is moving
	float currentDistance = glm::distance(entityToFollowTransform->getPosition() + this->offset, entityTransform->getPosition());

	if (currentDistance <= this->minDistance) {
		
		entityEuler->velocity = glm::vec3(0.0f);
		this->finished = true;
		return;
	}

	glm::vec3 idealCameraLocation = entityToFollowTransform->getPosition() + this->offset;
	glm::vec3 vVel = glm::normalize(idealCameraLocation);
	currentDistance = glm::distance(idealCameraLocation, entityTransform->getPosition());

	float distance = cScriptUtil::clamp(this->minDistance, this->maxSpeedDistance, currentDistance);
	float slowDownRatio = distance / (this->maxSpeedDistance - this->minDistance);
	slowDownRatio = cScriptUtil::smoothstep_wikipedia(0.0f, 1.0f, slowDownRatio);

	glm::vec3 direction = glm::normalize(entityToFollowTransform->getPosition() + this->offset - entityTransform->getPosition());
	entityEuler->velocity = (direction * slowDownRatio * this->maxSpeed);

	if(this->orientToTarget) {
		
		entityTransform->orientation =glm::inverse(glm::lookAt(
				entityTransform->getPosition(), entityToFollowTransform->getPosition() + this->offset,
				glm::vec3(0.0f,1.0f,0.0f)));
	}

	return;
}

bool cFollowEntity::isFinished() {
	return this->finished;
}

#include "cFadeEntityColorTrigger.h"
#include "cRenderMeshComponent.h"
#include "cTransformComponent.h"
#include "cSceneManager.h"

cFadeEntityColorTrigger::cFadeEntityColorTrigger(cEntity* entity, glm::vec4 color, float fadeDuration, float radius) : finished(false), isTriggered(false) {
	
	this->entity = entity;
	this->origColor = this->entity->getComponent<cRenderMeshComponent>()->materialDiffuse;
	this->destColor = color;
	this->fadeDuration = fadeDuration;
	this->radius = radius;
	this->elapsedTime = 0.0f;

	return;
}

void cFadeEntityColorTrigger::update(float deltaTime) {
	
	if(this->isTriggered) {
		
		this->elapsedTime += deltaTime;

		this->entity->getComponent<cRenderMeshComponent>()->materialDiffuse = glm::mix(this->origColor, this->destColor, this->elapsedTime / this->fadeDuration);

		if(elapsedTime > this->fadeDuration) {
			this->isTriggered = false;
			this->elapsedTime = 0.0f;
		}

		return;
	}

	cTransformComponent* triggerTransform = this->entity->getComponent<cTransformComponent>();
	glm::vec3 triggerPosition = triggerTransform->getPosition();
	float radius = this->radius;

	//check for trigger hit
	for (std::vector<cEntity*>::iterator it = cSceneManager::getInstance()->getActiveScene()->getEntities().begin(); it != cSceneManager::getInstance()->getActiveScene()->getEntities().end(); ++it) {
		
		if(*it == this->entity) {
			continue;
		}

		float distance = glm::distance((*it)->getComponent<cTransformComponent>()->getPosition(), triggerTransform->getPosition());
		if(distance <= 1.0f) {
			this->isTriggered = true;
		}

	}

	return;
}

bool cFadeEntityColorTrigger::isFinished() {
	return this->finished;
}

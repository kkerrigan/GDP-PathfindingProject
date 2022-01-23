#include "cParticleSystem.h"
#include "cParticleComponent.h"
#include "cTransformComponent.h"
#include "cRenderMeshComponent.h"
#include "cSceneManager.h"
#include "cShaderManager.h"
#include "cMeshRenderSystem.h"

cParticleSystem::cParticleSystem() {}

cParticleSystem::~cParticleSystem(){}

cParticleSystem* cParticleSystem::getInstance() {
	
	static cParticleSystem instance;

	return &instance;
}

void cParticleSystem::update(double deltaTime) {

	cScene* pScene = cSceneManager::getInstance()->getActiveScene();
	glm::vec3 cameraEye = pScene->flyCamera.eye;
	 
	cShaderManager::getInstance()->useShaderProgram("BasicUberShader"); // Don't know why this fixed it, but it do.
	cShaderManager::cShaderProgram* program = cShaderManager::getInstance()->pGetShaderProgramFromFriendlyName("BasicUberShader");

	// Black threshold is where the imposter will discard 
	glUniform1f(program->getUniformID_From_Name("ParticleImposterBlackThreshold"), 0.25f);

	for (unsigned int index = 0; index != this->entitiesToUpdate.size(); ++index) {

		cTransformComponent* pEntityTransform = this->entitiesToUpdate[index]->getComponent<cTransformComponent>();

		cParticleComponent* pCurParticle = this->entitiesToUpdate[index]->getComponent<cParticleComponent>();
		cEntity* pParticle = pCurParticle->particleEntity;
		cTransformComponent* pParticleTrans = pParticle->getComponent<cTransformComponent>();
		glm::vec3 oldPosition = pParticleTrans->getPosition();
		glm::quat oldOrientation = pParticleTrans->orientation;
		glm::vec3 oldScale = pParticleTrans->scale;
		pParticleTrans->setOrientationEulerAngles(0.0f, 0.0f, 0.0f);

		cRenderMeshComponent* pParticleMesh = pParticle->getComponent<cRenderMeshComponent>();
		pParticleMesh->bIsVisible = true;

		glm::mat4 matIdentity = glm::mat4(1.0f);

		glUniform1f(program->getUniformID_From_Name("bIsParticleImposter"), (float)GL_TRUE);

		pCurParticle->update(deltaTime);

		// Get the particles to draw
		std::vector<sParticle> vecParticlesToDraw;
		pCurParticle->getAliveParticles(vecParticlesToDraw);
		pCurParticle->sortParticlesBackToFront(vecParticlesToDraw, cameraEye);

		unsigned int numParticles = (unsigned int)vecParticlesToDraw.size();
		unsigned int count = 0;
		for (unsigned int index = 0; index != numParticles; ++index) {

			if (vecParticlesToDraw[index].lifeRemaining > 0.0f) {
				// Draw it
				pParticleTrans->setPosition(vecParticlesToDraw[index].position + pEntityTransform->getPosition());
				pParticleTrans->setUniformScale(vecParticlesToDraw[index].scale);
				pParticleTrans->setQOrientation(vecParticlesToDraw[index].qOrientation);

				// This is for the "death" transparency
				glUniform1f(program->getUniformID_From_Name("ParticleImposterAlphaOverride"), vecParticlesToDraw[index].transparency);

				cMeshRenderSystem::getInstance()->drawObject(pParticle, matIdentity);
				count++;
			}
		}

		//std::cout << "Drew " << count << " particles" << std::endl;
		pParticleMesh->bIsVisible = false;
		pParticleTrans->setPosition(oldPosition);
		pParticleTrans->setQOrientation(oldOrientation);
		pParticleTrans->scale = oldScale;
		glUniform1f(program->getUniformID_From_Name("bIsParticleImposter"), (float)GL_FALSE);
		glUniform1f(program->getUniformID_From_Name("ParticleImposterAlphaOverride"), 1.0f);
	}

	return;
}

void cParticleSystem::registerEntity(cEntity* entity) {

	this->entitiesToUpdate.push_back(entity);

	return;
}

void cParticleSystem::unregisterEntity(cEntity* entity) {
	// TODO: remove from vector

	return;
}


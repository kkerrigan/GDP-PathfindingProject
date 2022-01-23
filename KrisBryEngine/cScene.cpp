#include "cScene.h"
#include "Common.h"

#include <cstdio>
#include <fstream>
#include <sstream>
#include <iomanip>

#include <rapidjson/prettywriter.h>
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/istreamwrapper.h>

#include "cLightHelper.h"
#include "cShaderManager.h"
#include "cParticleComponent.h"
#include "cEulerMovementComponent.h"
#include "cTransformComponent.h"
#include "cRenderMeshComponent.h"
#include "cMeshRenderSystem.h"
#include "cRigidBodyComponent.h"

cScene::cScene() : cameraUp(UP), selectedEntity(0), selectedLight(0), selectedChannelGroup(0) {

	this->bIsLightDebug = false;
	this->bIsRenderDebug = false;
	this->bDisplayAABBS = false;
	this->bDisplayCamInfo = false;

	return;
}

cScene::~cScene(){}

void cScene::setSceneName(std::string name) {
	this->sceneName = name;
}

void cScene::updateWindowTitle(GLFWwindow* window) {

	cEntity* curObject = this->getSelectedEntity();
	cRenderMeshComponent* curMesh = curObject->getComponent<cRenderMeshComponent>();
	cTransformComponent* curTransform = curObject->getComponent<cTransformComponent>();
	
	sLight* curLight = this->getSelectedLight();
	glm::vec3 pos = curTransform->getPosition();

	// Set the window title
	std::stringstream ssTitle;
	ssTitle << "Camera (x,y,z) "
		<< " Eye "
		<< this->flyCamera.eye.x << ", " << this->flyCamera.eye.y << ", " << this->flyCamera.eye.z
		<< " Selected Model[" << curMesh->friendlyName << "] position: "
		<< pos.x << ", " << pos.y << ", " << pos.z
		<< " Selected Light[" << this->selectedLight << "] position: "
		<< curLight->position.x << ", " << curLight->position.y << ", " << curLight->position.z
		<< " Const: " << curLight->atten.x << " Linear: " << curLight->atten.y << " Quad: " << std::setprecision(9) << std::showpoint << std::fixed << curLight->atten.z;

	glfwSetWindowTitle(window, ssTitle.str().c_str());

	return;
}

bool cScene::getIsScenePaused() {
	return this->isScenePaused;
}

void cScene::setIsScenePaused(bool val) {
	this->isScenePaused = val;
}

void cScene::setCameraPosition(glm::vec3 pos) {
	this->cameraPos = pos;
}

void cScene::setCameraLookAt(glm::vec3 lookAt) {
	this->cameraLookAt = lookAt;
}

void cScene::addEntityToScene(cEntity* entity) {
	this->entities.push_back(entity);
}

void cScene::removeEntityFromScene(cEntity* entiy) {

}

cEntity* cScene::getSelectedEntity() {
	return this->entities[this->selectedEntity];
}

size_t cScene::getNumberOfEntites() {

	return this->entities.size();
}

cEntity* cScene::findObjectByFriendlyName(std::string theNameToFind) {

	for (unsigned int index = 0; index != this->entities.size(); index++) {

		cRenderMeshComponent* meshComp = this->entities[index]->getComponent<cRenderMeshComponent>();

		if (meshComp->friendlyName == theNameToFind)
		{
			return this->entities[index];
		}
	}

	return NULL;
}

cEntity* cScene::findObjectById(int id) {
	for (std::vector<cEntity*>::iterator it = this->entities.begin(); it != this->entities.end(); ++it) {
		
		if ((*it)->UNIQUE_ID == id) {
			return *it;
		}
	}

	return nullptr;
}

std::vector<cEntity*>& cScene::getEntities() {
	return this->entities;
}

void cScene::changeSelectedRigidBody(bool next) {

	if (next) {
		
		this->selectedEntity = (this->selectedEntity + 1) % this->entities.size();
	}
	else {
		
		this->selectedEntity -= 1;

		if (this->selectedEntity < 0) {
			
			this->selectedEntity = this->entities.size() - 1;
		}
	}

	//make sure it is a Rigid body
	if (entities[this->selectedEntity]->hasComponent<cRigidBodyComponent>() == false) {

		// if not call the function again
		this->changeSelectedRigidBody(next);
	}

	// For this case it must be a sphere
	cRigidBodyComponent* pPhysicsComp = entities[this->selectedEntity]->getComponent<cRigidBodyComponent>();
	if (pPhysicsComp->getRigidBody()->getShape()->getShapeType() != nPhysics::SHAPE_TYPE_SPHERE) {

		this->changeSelectedRigidBody(next);
	}

	return;
}

void cScene::addLightToScene(sLight* light) {
	this->lights.push_back(light);
}

void cScene::removeLightFromScene(sLight* light) {

}

sLight* cScene::getSelectedLight() {
	return this->lights[this->selectedLight];
}

sLight* cScene::getLightAt(unsigned int index) {
	return this->lights[index];
}

size_t cScene::getNumberOfLights() {
	return this->lights.size();
}

std::vector<sLight*> cScene::getLights() {
	return this->lights;
}

void cScene::addSoundToScene(cSoundManager::sSoundInfo* soundInfo) {
	this->sounds.push_back(soundInfo);
}

cSoundManager::sSoundInfo* cScene::getSoundInfo(unsigned index) {
	return this->sounds[index];
}

std::vector<cSoundManager::sSoundInfo*> cScene::getSounds() {
	return this->sounds;
}


double oldTime;
void cScene::handleCommand(iInputCommand* command) {

	double now = glfwGetTime();
	double delta = now - oldTime;
	oldTime = now;

	if (delta > 0.1f) {
		delta = 0.1f;
	}


	// camera controls
	cMoveCameraForwardCommand* moveCamForward = dynamic_cast<cMoveCameraForwardCommand*>(command);
	if (moveCamForward) {
		moveCamForward->execute(&this->flyCamera);
	}

	cMoveCameraLeftRightCommand* moveCamLeftRight = dynamic_cast<cMoveCameraLeftRightCommand*>(command);
	if (moveCamLeftRight) {
		moveCamLeftRight->execute(&this->flyCamera);
	}

	cMoveCameraUpDownCommand* moveCamYCommand = dynamic_cast<cMoveCameraUpDownCommand*>(command);
	if (moveCamYCommand) {
		moveCamYCommand->execute(&this->flyCamera);
	}

	cRollCameraCommand* rollCameraCommand = dynamic_cast<cRollCameraCommand*>(command);
	if (rollCameraCommand) {
		rollCameraCommand->execute(&this->flyCamera);
	}

	// light controls
	cMoveLightForwardCommand* moveLightForwardCommand = dynamic_cast<cMoveLightForwardCommand*>(command);
	if (moveLightForwardCommand) {
		moveLightForwardCommand->execute(this->getSelectedLight());
	}

	cMoveLightLeftRightCommand* moveLightLeftRightCommand = dynamic_cast<cMoveLightLeftRightCommand*>(command);
	if (moveLightLeftRightCommand) {
		moveLightLeftRightCommand->execute(this->getSelectedLight());
	}

	cMoveLightUpDownCommand* moveUpDownCommand = dynamic_cast<cMoveLightUpDownCommand*>(command);
	if (moveUpDownCommand) {
		moveUpDownCommand->execute(this->getSelectedLight());
	}

	//attenuation
	cLightChangeConstAttenuationCommand* changeLightConstAttenCommand = dynamic_cast<cLightChangeConstAttenuationCommand*>(command);
	if (changeLightConstAttenCommand) {
		changeLightConstAttenCommand->execute(this->getSelectedLight());
	}

	cLightChangeLinAttenuationCommand* changeLightLinAttenCommand = dynamic_cast<cLightChangeLinAttenuationCommand*>(command);
	if (changeLightLinAttenCommand) {
		changeLightLinAttenCommand->execute(this->getSelectedLight());
	}

	cLightChangeQuadAttenuationCommand* changeLightQuadAttenCommand = dynamic_cast<cLightChangeQuadAttenuationCommand*>(command);
	if (changeLightQuadAttenCommand) {
		changeLightQuadAttenCommand->execute(this->getSelectedLight());
	}
	
	// entity controls
	cTransformComponent* selectedEntityTransformComp = this->getSelectedEntity()->getComponent<cTransformComponent>();
	cRenderMeshComponent* selectedEntityMeshComp = this->getSelectedEntity()->getComponent<cRenderMeshComponent>();
	
	cMoveEntiyForwardCommand* moveEntityZCommand = dynamic_cast<cMoveEntiyForwardCommand*>(command);
	if (moveEntityZCommand) {
		moveEntityZCommand->execute(selectedEntityTransformComp);
	}

	cMoveEntiyLeftRightCommand* moveEntityXCommand = dynamic_cast<cMoveEntiyLeftRightCommand*>(command);
	if (moveEntityXCommand) {
		moveEntityXCommand->execute(selectedEntityTransformComp);
	}

	cMoveEntiyUpDownCommand* moveEntityYCommand = dynamic_cast<cMoveEntiyUpDownCommand*>(command);
	if (moveEntityYCommand) {
		moveEntityYCommand->execute(selectedEntityTransformComp);
	}

	cRotateEntiyXCommand* moveEntityRotXCommand = dynamic_cast<cRotateEntiyXCommand*>(command);
	if (moveEntityRotXCommand) {
		moveEntityRotXCommand->execute(selectedEntityTransformComp);
	}

	cRotateEntiyYCommand* moveEntityRotYCommand = dynamic_cast<cRotateEntiyYCommand*>(command);
	if (moveEntityRotYCommand) {
		moveEntityRotYCommand->execute(selectedEntityTransformComp);
	}

	cRotateEntiyZCommand* moveEntityRotZCommand = dynamic_cast<cRotateEntiyZCommand*>(command);
	if (moveEntityRotZCommand) {
		moveEntityRotZCommand->execute(selectedEntityTransformComp);
	}

	// specular power
	cChangeEntitySpecularPower* moveEntityChangeSpecPowerCommand = dynamic_cast<cChangeEntitySpecularPower*>(command);
	if (moveEntityChangeSpecPowerCommand) {
		moveEntityChangeSpecPowerCommand->execute(selectedEntityMeshComp);
	}

	//end the command's lifecycle
	delete command;
}

void cScene::setLightDebugSphere(cEntity* pSphere) {

	sLight* pCurLight = this->getSelectedLight();

	// Create debug object
	cTransformComponent* pSphereTrans = pSphere->getComponent<cTransformComponent>();
	cRenderMeshComponent* pSphereMesh = pSphere->getComponent<cRenderMeshComponent>();
	pSphereTrans->setPosition(glm::vec3(pCurLight->position));
	pSphereMesh->setDiffuseColour(glm::vec3(1.0f, 1.0f, 1.0f));
	pSphereTrans->setUniformScale(0.1f);

	glm::mat4 matBall = glm::mat4(1.0f);
	cMeshRenderSystem::getInstance()->drawObject(pSphere, matBall);

	cLightHelper* pLightHelper = new cLightHelper();

	// calculate 90% distance
	float distance90Percent = pLightHelper->calcApproxDistFromAtten(0.90f, this->ACCURACY_OF_DISTANCE, this->INFINITE_DISTANCE,
		pCurLight->atten.x, pCurLight->atten.y, pCurLight->atten.z);
	pSphereTrans->setUniformScale(distance90Percent);
	pSphereMesh->setDiffuseColour(glm::vec3(1.0f, 1.0f, 0.0f));
	cMeshRenderSystem::getInstance()->drawObject(pSphere, matBall);

	// calculate 50% brightness
	float distance50Percent = pLightHelper->calcApproxDistFromAtten(0.50f, this->ACCURACY_OF_DISTANCE, this->INFINITE_DISTANCE,
		pCurLight->atten.x, pCurLight->atten.y, pCurLight->atten.z);
	pSphereTrans->setUniformScale(distance50Percent);
	pSphereMesh->setDiffuseColour(glm::vec3(0.0f, 1.0f, 0.0f));
	cMeshRenderSystem::getInstance()->drawObject(pSphere, matBall);

	// calculate 25% brightness
	float distance25Percent = pLightHelper->calcApproxDistFromAtten(0.25f, this->ACCURACY_OF_DISTANCE, this->INFINITE_DISTANCE,
		pCurLight->atten.x, pCurLight->atten.y, pCurLight->atten.z);
	pSphereTrans->setUniformScale(distance25Percent);
	pSphereMesh->setDiffuseColour(glm::vec3(1.0f, 0.0f, 0.0f));
	cMeshRenderSystem::getInstance()->drawObject(pSphere, matBall);

	// calculate 1% brightness
	float distance1Percent = pLightHelper->calcApproxDistFromAtten(0.01f, this->ACCURACY_OF_DISTANCE, this->INFINITE_DISTANCE,
		pCurLight->atten.x, pCurLight->atten.y, pCurLight->atten.z);
	pSphereTrans->setUniformScale(distance1Percent);
	pSphereMesh->setDiffuseColour(glm::vec3(0.0f, 0.0f, 1.0f));
	cMeshRenderSystem::getInstance()->drawObject(pSphere, matBall);

	return;
}

// Animation Midterm Q1
void cScene::createMaze(int mazeWidth, int mazeHeight) {

	// Create the maze using the provided dimensions
	this->sceneMaze.GenerateMaze(mazeWidth, mazeHeight);

	// Iterate throgh the maze and place a cube wherever there is a wall
	const float PATH_WIDTH = 2.0f;
	const float X_OFFSET = -mazeWidth * PATH_WIDTH;
	const float Z_OFFSET = -mazeHeight * PATH_WIDTH;

	for (size_t idxA = 0; idxA != this->sceneMaze.maze.size(); ++idxA) {

		for (size_t idxB = 0; idxB != this->sceneMaze.maze[idxA].size(); ++idxB) {

			// Check if the current spot is a wall
			if (this->sceneMaze.maze[idxA][idxB][0] == true) {

				// Add a cube entity to represent the current wall
				cEntity* pWall = cEntityBuilder::getInstance()->createEntity(3);
				cTransformComponent* pWallTransComp = pWall->getComponent<cTransformComponent>();
				cRenderMeshComponent* pWallMeshComp = pWall->getComponent<cRenderMeshComponent>();

				pWallTransComp->setUniformScale(2.0f);

				float scaleFactor = pWallTransComp->scale.x;

				glm::vec3 pos = pWallTransComp->getPosition();
				pos.x = idxA * (PATH_WIDTH * scaleFactor) + X_OFFSET;
				pos.z = idxB * (PATH_WIDTH * scaleFactor) + Z_OFFSET;
				pWallTransComp->setPosition(pos);
				pWallMeshComp->bIsVisible = true;
				pWallMeshComp->bIsWireFrame = false;

				this->entities.push_back(pWall);
			}
			else {

				// Push this position into the empty squares vector to be used later
				this->emptySquares.push_back(glm::vec3(idxA * (PATH_WIDTH * 2.0f) + X_OFFSET, 0.0f, idxB * (PATH_WIDTH * 2.0f) + Z_OFFSET));
			}
		}
	}
	return;
}
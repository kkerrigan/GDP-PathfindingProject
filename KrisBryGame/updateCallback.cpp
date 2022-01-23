#include <cEngine.h>
#include <cConsole.h>
#include <cDebugRenderer.h>

#include <cSceneManager.h>
#include <cShaderManager.h>
#include <cFreeTypeRenderer.h>

#include <cTransformComponent.h>
#include <cRenderMeshComponent.h>
#include <cInstancedMeshComponent.h>
#include <cClothComponent.h>

#include <cBehaviourComponent.h>
#include <cFollowBehaviour.h>
#include <cCrowdFollowBehaviour.h>
#include <cFlockBehaviour.h>

#include <cMeshRenderSystem.h>
#include <cBehaviourSystem.h>

#include <physicsShapes.h>
#include <cColliderSystem.h>
#include <iInputCommand.h>
#include <cDebugUtilities.h>
#include <cPathFinding.h>


extern cEntity* pSphere;
extern cEntity* pCube;

extern cAABBTerrianBroadPhaseHierarchy g_AABBTerrianHierarchy;
extern cCrowdFollowBehaviour* pCrowdFollow;
extern cInstancedMeshComponent* instanceRenderer;

double nextLightSwitchTime = 0;

//void showModelNormals();
void checkFlockFollowPath();

float randomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

void colourSelectedRigidBody(cEntity* entity) {

	cScene* scene = cSceneManager::getInstance()->getActiveScene();

	//get the selected entity and the component we need
	cEntity* pEntity = scene->getSelectedEntity();

	cRenderMeshComponent* pMeshComp = pEntity->getComponent<cRenderMeshComponent>();
	if (!pMeshComp) return;

	if (entity == pEntity) {

		pMeshComp->setDiffuseColour(glm::vec3(1.0f, 1.0f, 1.0f));
	}
	else {
		pMeshComp->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
	}
}

void updateCallback(double deltaTime) {

	cShaderManager::getInstance()->useShaderProgram("BasicUberShader");
	GLint program = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");

	cSceneManager* pSceneManager = cSceneManager::getInstance();
	cScene* pScene = pSceneManager->getActiveScene();

	// Change the colour of the selected entity
	/*std::vector<cEntity*> entities = cSceneManager::getInstance()->getActiveScene()->getEntities();
	for (size_t index = 0; index != entities.size(); ++index) {

		colourSelectedRigidBody(entities[index]);
	}*/

	// Follow the selected entity
	cEntity* pSelectedEntity = pScene->getSelectedEntity();
	//pScene->flyCamera.followSelectedEntity(deltaTime, pSelectedEntity);


	// Debug render
	if (pScene->bIsLightDebug) {
		pScene->setLightDebugSphere(pSphere);
	}

	if (pScene->bDisplayCamInfo) {

		cDebugUtilities::renderCameraInfo();
	}

	// Render all of the nodes of the cloth soft body
	/*cEntity* cloth = pScene->findObjectByFriendlyName("Cloth");
	cClothComponent* clothComp = cloth->getComponent<cClothComponent>();
	nPhysics::iSoftBody* softBody = clothComp->getSoftBody();

	for (size_t index = 0; index < softBody->numNodes(); ++index) {

		cTransformComponent* clothTrans = cloth->getComponent<cTransformComponent>();
		glm::vec3 nodePos(0.0f);
		softBody->getNodePosition(index, nodePos);
		clothTrans->setPosition(nodePos);

		glm::mat4 matModel = glm::mat4(1.0f);
		cMeshRenderSystem::getInstance()->drawObject(cloth, matModel);
	}*/

	// Draw AABBs
	if (pScene->bDisplayAABBS) {

		cDebugUtilities::displaySceneAABBs(pCube);
	}

	// TODO: AI Project 3 - find a better spot for this
	//update the rendering stuff so the map represents what the ai sees
	instanceRenderer->updateInstances(pScene->pathfinding->path.pathNodes);
}
#include <cLightManager.h>
#include <cSceneManager.h>

void loadLights() {
	cSceneManager* sceneManager = cSceneManager::getInstance();
	cScene* pScene = sceneManager->getActiveScene();

	cLightManager* lightManager = cLightManager::getInstance();
	lightManager->initializeLights();

	// Graphics Final Question 3
	/*sLight* light1 = lightManager->getLightByIndex(1);
	light1->param2 = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	light1->position = glm::vec4(36.0f, 36.0f, 18.0f, 1.0f);
	light1->atten.y = 0.0005f;
	light1->atten.z = 0.00000000001f;
	light1->diffuse = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	pScene->addLightToScene(light1);*/

	/*sLight* light2 = lightManager->getLightByIndex(2);
	light2->param2 = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	light2->position = glm::vec4(39.0f, 36.0f, 18.0f, 1.0f);
	light2->atten.y = 0.000004779825758305378f;
	light2->atten.z = 2.3631906509399416f;
	light2->diffuse = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	pScene->addLightToScene(light2);*/
}
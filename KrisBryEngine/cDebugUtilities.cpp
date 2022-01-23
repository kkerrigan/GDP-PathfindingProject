#include "cDebugUtilities.h"
#include "cMeshRenderSystem.h"
#include "cFreeTypeRenderer.h"
#include "cSceneManager.h"
#include "cBehaviourComponent.h"
#include "cFlockBehaviour.h"
#include "cClothComponent.h"

void cDebugUtilities::renderCameraInfo() {

	cFreeTypeRenderer* textRenderer = cFreeTypeRenderer::getInstance();
	textRenderer->yoffset = 40;
	textRenderer->xoffset = 1.1f;

	const int left_column_width = -10;
	const int float_value_column_width = -7;

	cScene* scene = cSceneManager::getInstance()->getActiveScene();

	//buffer for output
	char out_string[100];

	sprintf_s(out_string, 100, "%-*s", left_column_width, "Camera Info:");
	textRenderer->renderText(out_string, -1 + textRenderer->xoffset, 1 - textRenderer->yoffset * textRenderer->sy, textRenderer->sx, textRenderer->sy, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	textRenderer->yoffset += 60;
	cDebugUtilities::clearBuffer(out_string, 100);


	glm::vec3 current_position = scene->flyCamera.eye;
	sprintf_s(out_string, 100, "%-*s x:%-*.2f y: %-*.2f z: %-*.2f", left_column_width, "Position:", float_value_column_width, current_position.x, float_value_column_width, current_position.y, float_value_column_width, current_position.z);
	textRenderer->renderText(out_string, -1 + textRenderer->xoffset, 1 - textRenderer->yoffset * textRenderer->sy, textRenderer->sx, textRenderer->sy, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	textRenderer->yoffset += 20;
	cDebugUtilities::clearBuffer(out_string, 100);

	glm::vec3 at = scene->flyCamera.getAtInWorldSpace();
	sprintf_s(out_string, 100, "%-*s x:%-*.2f y: %-*.2f z: %-*.2f", left_column_width, "At:", float_value_column_width, at.x, float_value_column_width, at.y, float_value_column_width, at.z);
	textRenderer->renderText(out_string, -1 + textRenderer->xoffset, 1 - textRenderer->yoffset * textRenderer->sy, textRenderer->sx, textRenderer->sy, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	textRenderer->yoffset += 20;
	cDebugUtilities::clearBuffer(out_string, 100);

	glm::vec3 up = scene->flyCamera.getUpVector();
	sprintf_s(out_string, 100, "%-*s x:%-*.2f y: %-*.2f z: %-*.2f", left_column_width, "Up:", float_value_column_width, up.x, float_value_column_width, up.y, float_value_column_width, up.z);
	textRenderer->renderText(out_string, -1 + textRenderer->xoffset, 1 - textRenderer->yoffset * textRenderer->sy, textRenderer->sx, textRenderer->sy, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	textRenderer->yoffset += 20;
	cDebugUtilities::clearBuffer(out_string, 100);


	return;
}

void cDebugUtilities::renderFlockInfo() {

	cFreeTypeRenderer* textRenderer = cFreeTypeRenderer::getInstance();
	textRenderer->yoffset = 40;
	textRenderer->xoffset = 0.1f;

	char out_string[100];

	std::vector<cEntity*> entities = cSceneManager::getInstance()->getActiveScene()->getEntities();

	for (size_t i = 0; i < entities.size(); ++i)
	{
		cBehaviourComponent* component = entities[i]->getComponent<cBehaviourComponent>();

		if (!component) continue;

		std::vector<iBehaviour*> behaviours = component->getBehaviours();
		bool is_done = false;
		for (size_t j = 0; j < behaviours.size(); ++j)
		{
			cFlockBehaviour* flock = dynamic_cast<cFlockBehaviour*>(behaviours[j]);
			sprintf_s(out_string, 100, "Cohesion: %.5f Separation: %.5f Alignment: %.5f", flock->cohesionWeight, flock->separationWeight, flock->alignmentWeight);
			textRenderer->renderText(out_string, -1 + textRenderer->xoffset, 1 - textRenderer->yoffset * textRenderer->sy, textRenderer->sx, textRenderer->sy, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			textRenderer->yoffset += 60;
			cDebugUtilities::clearBuffer(out_string, 100);

			sprintf_s(out_string, 100, "Flock Center Point: %.2f, %.2f, %.2f", flock->flockCenter.x, flock->flockCenter.y, flock->flockCenter.z);
			textRenderer->yoffset += 60;
			cDebugUtilities::clearBuffer(out_string, 100);
			is_done = true;
			break;
		}

		if (is_done) break;
	}

	return;
}

void cDebugUtilities::clearBuffer(char* buffer, size_t size) {

	for (size_t i = 0; i < size; ++i) {
		
		buffer[i] = '\0';
	}

	return;
}

void cDebugUtilities::displaySceneAABBs(cEntity* pCube) {

	// Set up bounding box object
	cTransformComponent* cubeTrans = pCube->getComponent<cTransformComponent>();
	cRenderMeshComponent* cubeMesh = pCube->getComponent<cRenderMeshComponent>();
	cubeMesh->bIsVisible = true;
	cubeMesh->friendlyName = "Cube";
	cubeMesh->bIsWireFrame = true;
	cubeMesh->bDontLight = true;
	cubeMesh->setDiffuseColour(glm::vec3(0.0f, 0.0f, 1.0f));

	cScene* pScene = cSceneManager::getInstance()->getActiveScene();
	std::vector<cEntity*> objects = pScene->getEntities();

	// Loop over all the objects in the scene and draw a bounding box around them
	for (size_t index = 0; index < objects.size(); ++index) {

		// First draw the large bounding box for the whole cloth
		cEntity* cloth = pScene->findObjectByFriendlyName("Cloth");
		if (cloth) {

			cClothComponent* pCloth = cloth->getComponent<cClothComponent>();
			nPhysics::iSoftBody* pSoftBody = pCloth->getSoftBody();

			// Get the AABB and node radius of the cloth
			glm::vec3 softBodyMin(FLT_MAX);
			glm::vec3 softBodyMax(-FLT_MAX);
			float nodeRadius = 0;

			pSoftBody->getAABB(softBodyMin, softBodyMax);
			pSoftBody->getNodeRadius(index, nodeRadius); // This assumes all nodes have the same radius

			// Calculate the scale of the AABB
			float scaleX = (softBodyMax.x - softBodyMin.x) / 2.0f;
			float scaleY = (softBodyMax.y - softBodyMin.y) / 2.0f;
			float scaleZ = (softBodyMax.z - softBodyMin.z) / 2.0f;

			// Calculate the center node of the cloth and place the box there
			glm::vec3 centre = (softBodyMax + softBodyMin) / 2.0f;
			cubeTrans->setPosition(centre);
			cubeTrans->scale = glm::vec3(scaleX, scaleY, scaleZ);

			// Now draw it
			glm::mat4x4 matModel(1.0f);
			cMeshRenderSystem::getInstance()->drawObject(pCube, matModel);
			
			// Now repeat the process for each node of the cloth
			for (size_t i = 0; i < pSoftBody->numNodes(); ++i) {

				// Sphere AABBs will be yellow
				cubeMesh->setDiffuseColour(glm::vec3(1.0f, 1.0f, 0.0f));

				// Get all the position information for this node
				glm::vec3 nodePositon(0.0f);
				float nodeRadius = 0.0f;

				pSoftBody->getNodePosition(i, nodePositon);
				pSoftBody->getNodeRadius(i, nodeRadius);

				// Set the cube using these values
				cubeTrans->setPosition(nodePositon);
				cubeTrans->setUniformScale(nodeRadius);

				// Draw it
				glm::mat4x4 mat(1.0f);
				cMeshRenderSystem::getInstance()->drawObject(pCube, mat);
			}
		}
	}

	return;
}

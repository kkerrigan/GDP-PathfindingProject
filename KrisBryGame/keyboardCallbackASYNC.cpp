#include <Global.h>
#include <cSceneManager.h>
#include <cEntityBuilder.h>
#include <cTransformComponent.h>
#include <cRenderMeshComponent.h>
#include <cRigidBodyComponent.h>
#include <cSkinnedMeshComponent.h>
#include <cBehaviourComponent.h>
#include <cFlockBehaviour.h>
#include <iInputCommand.h>

#include <cMeshRenderSystem.h>

bool isShiftDown(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT)) { return true; }
	// both are up
	return false;
}

bool isCtrlDown(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL)) { return true; }
	// both are up
	return false;
}

bool isAltDown(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT)) { return true; }
	// both are up
	return false;
}

bool areAllModifiersUp(GLFWwindow* window) {
	if (isShiftDown(window)) { return false; }
	if (isCtrlDown(window)) { return false; }
	if (isAltDown(window)) { return false; }
	// Yup, they are all UP
	return true;
}

void keyboardCallbackASYNC(GLFWwindow* window, float deltaTime) {
	/*
	* All keypresses get sent to the "mediator" (scene) and are handled there. The command do no know the
	* actual object they are going to act on until they are executed
	*/
	
	cScene* pScene = cSceneManager::getInstance()->getActiveScene();
	
	const float CAMERA_SPEED_SLOW = 0.1f;
	const float CAMERA_SPEED_FAST = 1.0f;

	float cameraSpeed = CAMERA_SPEED_SLOW;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		cameraSpeed = CAMERA_SPEED_FAST;
	}

	cEntity* pSelectedEntity = pScene->getSelectedEntity();
	std::vector<cEntity*> entities = pScene->getEntities();

	// If no keys are down, move the camera
	if (areAllModifiersUp(window)) {

		// Flock variable modifiers
		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {

			for (size_t i = 0; i < entities.size(); ++i) {

				cBehaviourComponent* component = entities[i]->getComponent<cBehaviourComponent>();

				if (!component) continue;

				std::vector<iBehaviour*> behaviours = component->getBehaviours();

				for (size_t j = 0; j < behaviours.size(); ++j) {

					cFlockBehaviour* flock = dynamic_cast<cFlockBehaviour*>(behaviours[j]);
					flock->cohesionWeight < 1.0f ? flock->cohesionWeight += 0.001f : flock->cohesionWeight = 1.0f;
				}
			}
		}

		if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {

			for (size_t i = 0; i < entities.size(); ++i) {

				cBehaviourComponent* component = entities[i]->getComponent<cBehaviourComponent>();

				if (!component) continue;

				std::vector<iBehaviour*> behaviours = component->getBehaviours();

				for (size_t j = 0; j < behaviours.size(); ++j) {

					cFlockBehaviour* flock = dynamic_cast<cFlockBehaviour*>(behaviours[j]);
					flock->cohesionWeight <= 0.0f ? flock->cohesionWeight = 0.0f : flock->cohesionWeight -= 0.001f;
				}
			}
		}

		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
			
			for (size_t i = 0; i < entities.size(); ++i) {
				
				cBehaviourComponent* component = entities[i]->getComponent<cBehaviourComponent>();

				if (!component) continue;

				std::vector<iBehaviour*> behaviours = component->getBehaviours();

				for (size_t j = 0; j < behaviours.size(); ++j) {
					
					cFlockBehaviour* flock = dynamic_cast<cFlockBehaviour*>(behaviours[j]);
					flock->separationWeight < 1.0f ? flock->separationWeight += 0.001f : flock->separationWeight = 1.0f;
				}
			}
		}

		if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {

			for (size_t i = 0; i < entities.size(); ++i) {
				
				cBehaviourComponent* component = entities[i]->getComponent<cBehaviourComponent>();

				if (!component) continue;

				std::vector<iBehaviour*> behaviours = component->getBehaviours();

				for (size_t j = 0; j < behaviours.size(); ++j) {
					
					cFlockBehaviour* flock = dynamic_cast<cFlockBehaviour*>(behaviours[j]);
					flock->separationWeight <= 0.0f ? flock->separationWeight = 0.0f : flock->separationWeight -= 0.001f;
				}
			}
		}

		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {

			for (size_t i = 0; i < entities.size(); ++i) {
				
				cBehaviourComponent* component = entities[i]->getComponent<cBehaviourComponent>();

				if (!component) continue;

				std::vector<iBehaviour*> behaviours = component->getBehaviours();

				for (size_t j = 0; j < behaviours.size(); ++j) {
					
					cFlockBehaviour* flock = dynamic_cast<cFlockBehaviour*>(behaviours[j]);
					flock->alignmentWeight < 1.0f ? flock->alignmentWeight += 0.001f : flock->alignmentWeight = 1.0f;
				}
			}
		}

		if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {

			for (size_t i = 0; i < entities.size(); ++i) {
				
				cBehaviourComponent* component = entities[i]->getComponent<cBehaviourComponent>();

				if (!component) continue;

				std::vector<iBehaviour*> behaviours = component->getBehaviours();

				for (size_t j = 0; j < behaviours.size(); ++j) {
					
					cFlockBehaviour* flock = dynamic_cast<cFlockBehaviour*>(behaviours[j]);
					flock->alignmentWeight <= 0.0f ? flock->alignmentWeight = 0.0f : flock->alignmentWeight -= 0.001f;
				}
			}
		} // End of Flock variable modifiers



		// Get the physics component to apply force to the rigid body
		const float appliedForce = 25.0f;
		cRigidBodyComponent* pRigidBodyComp = pScene->getSelectedEntity()->getComponent<cRigidBodyComponent>();
		nPhysics::iRigidBody* pBody = nullptr;

		if (pRigidBodyComp != nullptr) {

			pBody = pRigidBodyComp->getRigidBody();
		}

		glm::vec3 forwardVec = glm::normalize(pScene->flyCamera.getAtInWorldSpace() - pScene->flyCamera.eye);
		forwardVec.y = 0.0f;

		glm::vec3 leftCameraDirection = glm::normalize(glm::cross(forwardVec, pScene->flyCamera.getUpVector()));
		forwardVec.y = 0.0f;

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {

			if (pBody != nullptr) {

				pBody->applyFoce(glm::vec3(appliedForce) * forwardVec);
			}
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {

			if (pBody != nullptr) {

				pBody->applyFoce(glm::vec3(appliedForce) * -forwardVec);
			}
		}

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {

			if (pBody != nullptr) {

				pBody->applyFoce(glm::vec3(appliedForce) * -leftCameraDirection);
			}
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {

			if (pBody != nullptr) {

				pBody->applyFoce(glm::vec3(appliedForce) * leftCameraDirection);
			}
		}


		if (glfwGetKey(window, GLFW_KEY_I)) {
			
			pScene->handleCommand(new cMoveCameraForwardCommand(5.0f * cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_K)) {
			
			pScene->handleCommand(new cMoveCameraForwardCommand(5.0f * -cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_J)) {
			
			pScene->handleCommand(new cMoveCameraLeftRightCommand(5.0f * -cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_L)) {
			
			pScene->handleCommand(new cMoveCameraLeftRightCommand(5.0f * cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			pScene->handleCommand(new cMoveCameraUpDownCommand(-cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
			pScene->handleCommand(new cMoveCameraUpDownCommand(cameraSpeed));
		}
	}

	// Shift down? Manipulate light objects
	if (isShiftDown(window)) {

		sLight* selectedLight = pScene->getSelectedLight();

		if (glfwGetKey(window, GLFW_KEY_W)) { // Forward
			pScene->handleCommand(new cMoveLightForwardCommand(cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_S)) { // Backward
			pScene->handleCommand(new cMoveLightForwardCommand(-cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_A)) {  // Left
			pScene->handleCommand(new cMoveLightLeftRightCommand(-cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_D)) { // Right
			pScene->handleCommand(new cMoveLightLeftRightCommand(cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_Q)) { // Up
			pScene->handleCommand(new cMoveLightUpDownCommand(cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_E)) { // Down
			pScene->handleCommand(new cMoveLightUpDownCommand(-cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_1)) { // Decrease Const	
			pScene->handleCommand(new cLightChangeConstAttenuationCommand(0.99f));
		}

		if (glfwGetKey(window, GLFW_KEY_2)) { //Increase Const	
			pScene->handleCommand(new cLightChangeConstAttenuationCommand(1.01f));
		}

		if (glfwGetKey(window, GLFW_KEY_3)) { // Decrease Linear	
			pScene->handleCommand(new cLightChangeLinAttenuationCommand(0.99f));
		}

		if (glfwGetKey(window, GLFW_KEY_4)) { // Increase Linear	
			pScene->handleCommand(new cLightChangeLinAttenuationCommand(1.01f));
		}

		if (glfwGetKey(window, GLFW_KEY_5)) { // Decrease Quadratic	
			pScene->handleCommand(new cLightChangeQuadAttenuationCommand(0.99f));
		}

		if (glfwGetKey(window, GLFW_KEY_6)) { // Increase Quadratic	
			pScene->handleCommand(new cLightChangeQuadAttenuationCommand(1.01f));
		}
	}

	// Alt down? Manipultate Enity objects
	if (isAltDown(window)) {
		
		cTransformComponent* selectedEntityTransformComp = pSelectedEntity->getComponent<cTransformComponent>();
		cRenderMeshComponent* selectedEntityMeshComp = pSelectedEntity->getComponent<cRenderMeshComponent>();

		if (glfwGetKey(window, GLFW_KEY_W)) {
			pScene->handleCommand(new cMoveEntiyForwardCommand(cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_S)) {
			pScene->handleCommand(new cMoveEntiyForwardCommand(-cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_A)) {
			pScene->handleCommand(new cMoveEntiyLeftRightCommand(cameraSpeed));
		}
		if (glfwGetKey(window, GLFW_KEY_D)) {
			pScene->handleCommand(new cMoveEntiyLeftRightCommand(-cameraSpeed));
		}
		if (glfwGetKey(window, GLFW_KEY_Q)) {
			pScene->handleCommand(new cMoveEntiyUpDownCommand(cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_E)) {
			pScene->handleCommand(new cMoveEntiyUpDownCommand(-cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_KP_ADD)) {
			pScene->handleCommand(new cChangeEntitySpecularPower(1.01f));
		}

		if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT)) {
			pScene->handleCommand(new cChangeEntitySpecularPower(0.99f));
		}

		if (glfwGetKey(window, GLFW_KEY_I)) {
			pScene->handleCommand(new cRotateEntiyXCommand(5.0f * cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_K)) {
			pScene->handleCommand(new cRotateEntiyXCommand(-5.0f * cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_U)) {
			pScene->handleCommand(new cRotateEntiyYCommand(5.0f * cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_O)) {
			pScene->handleCommand(new cRotateEntiyYCommand(-5.0f * cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_J)) {
			pScene->handleCommand(new cRotateEntiyZCommand(5.0f * cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_L)) {
			pScene->handleCommand(new cRotateEntiyZCommand(-5.0f * cameraSpeed));
		}

		// Scale
		if (glfwGetKey(window, GLFW_KEY_PERIOD)) {

			cEntity* pEntity = pScene->getSelectedEntity();
			cTransformComponent* pTransComp = pEntity->getComponent<cTransformComponent>();

			pTransComp->scale *= 1.01f;
		}

		if (glfwGetKey(window, GLFW_KEY_COMMA)) {

			cEntity* pEntity = pScene->getSelectedEntity();
			cTransformComponent* pTransComp = pEntity->getComponent<cTransformComponent>();

			pTransComp->scale *= 0.99f;
		}
	}

	// Is Ctrl down? Other operations
	if (isCtrlDown(window)) {

		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) { // Roll camera to the left
			pScene->handleCommand(new cRollCameraCommand(-cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) { // Roll camera to the right
			pScene->handleCommand(new cRollCameraCommand(+cameraSpeed));
		}
	}
}
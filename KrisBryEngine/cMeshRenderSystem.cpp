#include <algorithm>

#include "cEngine.h"
#include "cMeshRenderSystem.h"
#include "cRenderMeshComponent.h"
#include "cTransformComponent.h"
#include "cRigidBodyComponent.h"
#include "cFrameBufferComponent.h"
#include "cSkinnedMeshComponent.h"
#include "cInstancedMeshComponent.h"
#include "cVAOMeshManager.h"
#include "sLight.h"
#include "cShaderManager.h"
#include "cSceneManager.h"
#include "cDebugRenderer.h"
#include "cBasicTextureManager.h"

#include "cFBOSystem.h"

class cPredEntityIsCloser {
public:
	cPredEntityIsCloser(glm::vec3 cameraEye_) {
		this->cameraEye = cameraEye_;
		return;
	}
	glm::vec3 cameraEye;

	bool operator() (cEntity* P1, cEntity* P2) {

		cTransformComponent* entityOneTrans = P1->getComponent<cTransformComponent>();
		cTransformComponent* entityTwoTrans = P2->getComponent<cTransformComponent>();
		cRenderMeshComponent* entityOneMesh = P1->getComponent<cRenderMeshComponent>();
		cRenderMeshComponent* entityTwoMesh = P2->getComponent<cRenderMeshComponent>();

		float P1_dist_to_camera = glm::distance(entityOneTrans->getPosition(), this->cameraEye);
		float P2_dist_to_camera = glm::distance(entityTwoTrans->getPosition(), this->cameraEye);

		return P1_dist_to_camera > P2_dist_to_camera;
	}
};


cMeshRenderSystem::cMeshRenderSystem() {

	this->skyBoxTextureUnitID = 30;

	return;
}

cMeshRenderSystem::~cMeshRenderSystem() {}

cMeshRenderSystem* cMeshRenderSystem::getInstance() {
	static cMeshRenderSystem instance;

	return &instance;
}

void cMeshRenderSystem::update(double deltaTime) {

	cSceneManager* sceneManager = cSceneManager::getInstance();
	cScene* activeScene = sceneManager->getActiveScene();

	// Switch to the shader we want
	cShaderManager::getInstance()->useShaderProgram("BasicUberShader");
	cShaderManager::cShaderProgram* program = cShaderManager::getInstance()->pGetShaderProgramFromFriendlyName("BasicUberShader");

	float ratio;
	int width, height;
	glm::mat4x4 matProjection = glm::mat4(1.0f);
	glm::mat4x4	matView = glm::mat4(1.0f);

	glfwGetFramebufferSize(cEngine::getInstance()->getWindow(), &width, &height);
	ratio = width / (float)height;
	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH);			 // Enables the KEEPING of the depth information
	glEnable(GL_DEPTH_TEST);	// When drawing, checked the existing depth
	glEnable(GL_CULL_FACE);		// Discared "back facing" triangles

	matProjection = glm::perspective(0.6f,
		ratio,			// Aspect ratio
		0.1f,			// Near clipping plane
		10000.0f);		// Far clipping plane

	matView = glm::lookAt(activeScene->flyCamera.eye, activeScene->flyCamera.getAtInWorldSpace(), activeScene->flyCamera.getUpVector());

	glUniformMatrix4fv(program->getUniformID_From_Name("matView"), 1, GL_FALSE, glm::value_ptr(matView));
	glUniformMatrix4fv(program->getUniformID_From_Name("matProj"), 1, GL_FALSE, glm::value_ptr(matProjection));

	// Draw Sky box first
	this->drawSkyBox();

	// Draw current scene
	this->drawScene_Simple(this->entitiesToUpdate, 0);

	// Update any FBO objects
	cFBOSystem::getInstance()->update(deltaTime);

	cDebugRenderer::getInstance()->RenderDebugObjects(matView, matProjection, deltaTime);
}

void cMeshRenderSystem::drawSkyBox() {

	cShaderManager::cShaderProgram* program = cShaderManager::getInstance()->pGetShaderProgramFromFriendlyName("BasicUberShader");
	cScene* activeScene = cSceneManager::getInstance()->getActiveScene();
	cEntity* pSkyBox = activeScene->findObjectByFriendlyName("SkyBoxObject");
	cTransformComponent* pSkyboxTransComp = pSkyBox->getComponent<cTransformComponent>();
	cRenderMeshComponent* pSkyboxMeshComp = pSkyBox->getComponent<cRenderMeshComponent>();

	// Draw the skybox first 
	pSkyboxTransComp->setPosition(activeScene->flyCamera.eye);
	pSkyboxMeshComp->bIsVisible = true;
	pSkyboxMeshComp->bIsWireFrame = false;

	// Bind the cube map texture to the cube map in the shader
	glActiveTexture(this->skyBoxTextureUnitID + GL_TEXTURE0);
	int cubeMapTextureID = cBasicTextureManager::getInstance()->getTextureIDFromName("TropicalDayCubeMap");
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID);
	glUniform1i(program->getUniformID_From_Name("textureSkyBox"), this->skyBoxTextureUnitID);

	glUniform1f(program->getUniformID_From_Name("useSkyBoxTexture"), (float)GL_TRUE);

	glm::mat4 matIdentity = glm::mat4(1.0f);

	this->drawObject(pSkyBox, matIdentity);

	pSkyboxMeshComp->bIsVisible = false;
	glUniform1f(program->getUniformID_From_Name("useSkyBoxTexture"), (float)GL_FALSE);

	return;
}

void cMeshRenderSystem::registerEntity(cEntity* entity) {
	this->entitiesToUpdate.push_back(entity);
}

void cMeshRenderSystem::unregisterEntity(cEntity* entity) {
	// TODO: remove from vector
}

void cMeshRenderSystem::bindTextures(cEntity* pCurrentEntity) {

	cShaderManager::cShaderProgram* program = cShaderManager::getInstance()->pGetShaderProgramFromFriendlyName("BasicUberShader");

	// ******************************************************************** 
	//    _  _              _ _       ___ ___  ___    _           _                  _    _         _ _           
	//   | || |__ _ _ _  __| | |___  | __| _ )/ _ \  | |_ _____ _| |_ _  _ _ _ ___  | |__(_)_ _  __| (_)_ _  __ _ 
	//   | __ / _` | ' \/ _` | / -_) | _|| _ \ (_) | |  _/ -_) \ /  _| || | '_/ -_) | '_ \ | ' \/ _` | | ' \/ _` |
	//   |_||_\__,_|_||_\__,_|_\___| |_| |___/\___/   \__\___/_\_\\__|\_,_|_| \___| |_.__/_|_||_\__,_|_|_||_\__, |
	//                                                                                                      |___/ 

	//Check to see if the current mesh has a cFrameBufferComponent.....
	// If it DOES, then we bind the texture to that, and exit.
	// We will be making this more sophisticated so that we can have
	cFrameBufferComponent* pCurrFBOComp = pCurrentEntity->getComponent<cFrameBufferComponent>();
	if (pCurrFBOComp != nullptr) {

		// Connect the texture for this object to the FBO texture
		int currFboTextureUnit = pCurrFBOComp->getFboTextureUnit();
		glActiveTexture(GL_TEXTURE0 + currFboTextureUnit);

		// Connect the specific texture to THIS texture unit
		glBindTexture(GL_TEXTURE_2D, pCurrFBOComp->pFbo->colourTexture_0_ID);

		// Set the sampler (in the shader) to ALSO point to the texture unit
		// This one takes the unchanged texture unit numbers
		glUniform1i(program->getUniformID_From_Name("texPass1OutputTexture"), currFboTextureUnit);

		// NOTE: Early return (so we don't set any other textures
		// HACK!!
		return;
	} // if (pCurrFBOComp != nullptr)

	float blendWeights[8] = { 0 };
	cRenderMeshComponent* pCurrentMesh = pCurrentEntity->getComponent<cRenderMeshComponent>();

	for (int texBindIndex = 0; texBindIndex != pCurrentMesh->textures.size(); texBindIndex++) {

		// Bind to the the "texBindIndex" texture unit
		glActiveTexture(GL_TEXTURE0 + texBindIndex);

		// Connect the specific texture to THIS texture unit
		std::string texName = pCurrentMesh->textures[texBindIndex].name;

		GLuint texID = cBasicTextureManager::getInstance()->getTextureIDFromName(texName);

		glBindTexture(GL_TEXTURE_2D, texID);

		// Use a switch to pick the texture sampler and weight (strength)
		// BECAUSE the samplers can't be in an array
		switch (texBindIndex)
		{
		case 0:		// uniform sampler2D texture00  AND texBlendWeights[0].x;
			glUniform1i(program->getUniformID_From_Name("texture00"), texBindIndex);
			break;
		case 1:		// uniform sampler2D texture01  AND texBlendWeights[0].y;
			glUniform1i(program->getUniformID_From_Name("texture01"), texBindIndex);
			break;
		case 2:
			glUniform1i(program->getUniformID_From_Name("texture02"), texBindIndex);
			break;
		case 3:
			glUniform1i(program->getUniformID_From_Name("texture03"), texBindIndex);
			break;
		case 4:		// uniform sampler2D texture04  AND texBlendWeights[1].x;
			glUniform1i(program->getUniformID_From_Name("texture04"), texBindIndex);
			break;
		case 5:
			glUniform1i(program->getUniformID_From_Name("texture05"), texBindIndex);
			break;
		case 6:
			glUniform1i(program->getUniformID_From_Name("texture06"), texBindIndex);
			break;
		case 7:
			glUniform1i(program->getUniformID_From_Name("texture07"), texBindIndex);
			break;
		}//switch ( texBindIndex )

		// Set the blend weight (strengt)
		blendWeights[texBindIndex] = pCurrentMesh->textures[texBindIndex].strength;
	}

	// Set the weights (strengths) in the shader
	glUniform4f(program->getUniformID_From_Name("texBlendWeights[0]"), blendWeights[0], blendWeights[1], blendWeights[2], blendWeights[3]);
	glUniform4f(program->getUniformID_From_Name("texBlendWeights[1]"), blendWeights[4], blendWeights[5], blendWeights[6], blendWeights[7]);

	return;
}


void cMeshRenderSystem::drawObject(cEntity* pEntity, glm::mat4x4 &matModel) {

	cShaderManager::cShaderProgram* program = cShaderManager::getInstance()->pGetShaderProgramFromFriendlyName("BasicUberShader");
	cScene* activeScene = cSceneManager::getInstance()->getActiveScene();
	cTransformComponent* transformComponent = pEntity->getComponent<cTransformComponent>();
	cRenderMeshComponent* meshComponent = pEntity->getComponent<cRenderMeshComponent>();
	cRigidBodyComponent* physicsComp = pEntity->getComponent<cRigidBodyComponent>();

	glUseProgram(program->ID);

	// Is this object visible
	if (!meshComponent->bIsVisible) {
		return;
	}

	//is this an instanced mesh
	if (pEntity->hasComponent<cInstancedMeshComponent>()) {

		this->drawObjectInstances(pEntity);
		return;
	}

	this->bindTextures(pEntity);

	matModel = glm::mat4x4(1.0f);

	glm::mat4 matTranslation;
	glm::quat orientation;

	if (physicsComp != nullptr && physicsComp->getRigidBody() != nullptr) {

		nPhysics::iRigidBody* pBody = physicsComp->getRigidBody();
		glm::vec3 bodyPosition(0.0f);

		pBody->getPosition(bodyPosition);
		pBody->getOrientation(orientation);
		matTranslation = glm::translate(glm::mat4(1.0f), bodyPosition);

	}
	else {
		matTranslation = glm::translate(glm::mat4(1.0f), transformComponent->getPosition());
		orientation = transformComponent->orientation;
	}

	matModel = matModel * matTranslation;

	glm::mat4 matQrotation = glm::mat4(orientation);
	matModel *= matQrotation;

	// Calculate the inverse transpose before the scaling
	glm::mat4 matModelInvTrans = glm::inverse(glm::transpose(matModel));

	// And now scale
	glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
		transformComponent->scale);
	matModel = matModel * matScale;

	glUniformMatrix4fv(program->getUniformID_From_Name("matModel"), 1, GL_FALSE, glm::value_ptr(matModel));
	glUniformMatrix4fv(program->getUniformID_From_Name("matModelInvTrans"), 1, GL_FALSE, glm::value_ptr(matModelInvTrans));

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Take the 4th value from the material and pass it as alpha
	glUniform1f(program->getUniformID_From_Name("wholeObjectAlphaTransparency"), meshComponent->materialDiffuse.a);

	glUniform4f(program->getUniformID_From_Name("objectDiffuse"),
		meshComponent->materialDiffuse.r,
		meshComponent->materialDiffuse.g,
		meshComponent->materialDiffuse.b,
		meshComponent->materialDiffuse.a);

	glUniform3f(program->getUniformID_From_Name("eyeLocation"), activeScene->flyCamera.eye.x, activeScene->flyCamera.eye.y, activeScene->flyCamera.eye.z);
	glUniform4f(program->getUniformID_From_Name("objectSpecular"), meshComponent->materialSpecular.r, meshComponent->materialSpecular.g, meshComponent->materialSpecular.b, meshComponent->materialSpecular.a);

	if (meshComponent->bUseVertexColour)
	{
		glUniform1f(program->getUniformID_From_Name("useVertexColour"), (float)GL_TRUE);
	}
	else
	{
		glUniform1f(program->getUniformID_From_Name("useVertexColour"), (float)GL_FALSE);
	}

	/*if (meshComponent->colorByHeight) {
		glUniform1f(this->colorByHeight_UniLoc, (float)GL_TRUE);
	}
	else {
		glUniform1f(this->colorByHeight_UniLoc, (float)GL_FALSE);
	}*/

	if (meshComponent->bDontLight)
	{
		glUniform1f(program->getUniformID_From_Name("bDontUseLighting"), (float)GL_TRUE);
	}
	else
	{
		glUniform1f(program->getUniformID_From_Name("bDontUseLighting"), (float)GL_FALSE);
	}

	if (meshComponent->bIsWireFrame)
	{
		// Yes, draw it wireframe
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_CULL_FACE);	// Discared "back facing" triangles
		//glDisable( GL_DEPTH );		// Enables the KEEPING of the depth information
		//glDisable( GL_DEPTH_TEST );	// When drawing, checked the existing depth
	}
	else
	{
		// No, it's "solid" (or "Filled")
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_CULL_FACE);	// Discared "back facing" triangles
		//glEnable( GL_DEPTH );		// Enables the KEEPING of the depth information
		//glEnable( GL_DEPTH_TEST );	// When drawing, checked the existing depth
	}

	// Skinned Mesh start
	// Skinned mesh or regular?
	sModelDrawInfo modelInfo;

	if (!pEntity->hasComponent<cSkinnedMeshComponent>()) {

		// Regular mesh
		modelInfo.meshFileName = meshComponent->meshName;
		glUniform1f(program->getUniformID_From_Name("bIsASkinnedMesh"), (float)GL_FALSE);
	}
	else {
		// Skinned mesh
		cSkinnedMeshComponent* skinMeshComp = pEntity->getComponent<cSkinnedMeshComponent>();
		modelInfo.meshFileName = skinMeshComp->pSkinnedMesh->fileName;
		glUniform1f(program->getUniformID_From_Name("bIsASkinnedMesh"), (float)GL_TRUE);

		// Pass up the bone information
		std::vector<glm::mat4x4> vecFinalTransformation;
		std::vector<glm::mat4x4> vecOffsets;

		// Animation Midterm Q6
		// Get the currently running animation from the skin mesh
		cAnimationState::sStateDetails& runningAnimation = skinMeshComp->pAniState->getRunningAnimation();

		// Update the bone transforms for this frame of the animation
		runningAnimation.incrementTime(0.006f);
		skinMeshComp->pSkinnedMesh->boneTransform(runningAnimation.elapsedTime, //curFrameTime
			runningAnimation.name, // animation to play
			vecFinalTransformation, // Final bone transforms for mesh
			skinMeshComp->vecObjectBoneTransformation, // final location of bones
			vecOffsets); // local offset for each bone



		unsigned int numberOfBonesUsed = static_cast<unsigned int>(vecFinalTransformation.size());
		glUniform1f(program->getUniformID_From_Name("numBonesUsed"), numberOfBonesUsed);

		glm::mat4x4* pBoneMatrixArray = &(vecFinalTransformation[0]);

		glUniformMatrix4fv(program->getUniformID_From_Name("bones"), numberOfBonesUsed, GL_FALSE,
			(const GLfloat*)glm::value_ptr(*pBoneMatrixArray));


		// Update the extents of the skinned mesh from the bones...
		for (unsigned int boneIndex = 0; boneIndex != numberOfBonesUsed; ++boneIndex) {

			glm::mat4 boneLocal = skinMeshComp->vecObjectBoneTransformation[boneIndex];

			// HACK: Need to add "uniform scale" to the mesh
			//float scale = transformComponent->scale.x;
			float scale = 1.0f; // For now

			boneLocal = glm::scale(boneLocal, glm::vec3(transformComponent->scale.x,
				transformComponent->scale.y,
				transformComponent->scale.z));

			glm::vec4 boneBallLocation = boneLocal * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			boneBallLocation *= scale;

			// Bone index [13] = "B_L_Finger31"
	//		if (boneIndex == 25)
	//		{
	//			//DrawDebugBall( glm::vec3(boneBallLocation), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 0.5f );
	////			std::cout << "Bone 13, B_L_Finger31: " 
	////				<< boneBallLocation.x << ", "
	////				<< boneBallLocation.y << ", " 
	////				<< boneBallLocation.z << std::endl;

	//			cMeshSceneObject* pPlayerBody = findObjectByFriendlyName("PlayerBody");
	//			pPlayerBody->setUniformScale(10.0f);
	//			pPlayerBody->position = boneBallLocation;
	//		}

			// Update extents
			if (boneIndex == 0) {

				// For the 0th bone, just assume this is the extent
				skinMeshComp->minXYZ_from_SM_Bones = glm::vec3(boneBallLocation);
				skinMeshComp->maxXYZ_from_SM_Bones = glm::vec3(boneBallLocation);
			}
			else {
				// It's not the 0th bone, so compare with current max and min
				if (skinMeshComp->minXYZ_from_SM_Bones.x < boneBallLocation.x) { skinMeshComp->minXYZ_from_SM_Bones.x = boneBallLocation.x; }
				if (skinMeshComp->minXYZ_from_SM_Bones.y < boneBallLocation.y) { skinMeshComp->minXYZ_from_SM_Bones.y = boneBallLocation.y; }
				if (skinMeshComp->minXYZ_from_SM_Bones.z < boneBallLocation.z) { skinMeshComp->minXYZ_from_SM_Bones.z = boneBallLocation.z; }

				if (skinMeshComp->maxXYZ_from_SM_Bones.x > boneBallLocation.x) { skinMeshComp->maxXYZ_from_SM_Bones.x = boneBallLocation.x; }
				if (skinMeshComp->maxXYZ_from_SM_Bones.y > boneBallLocation.y) { skinMeshComp->maxXYZ_from_SM_Bones.y = boneBallLocation.y; }
				if (skinMeshComp->maxXYZ_from_SM_Bones.z > boneBallLocation.z) { skinMeshComp->maxXYZ_from_SM_Bones.z = boneBallLocation.z; }
			}

		} // for (unsigned int boneIndex = 0; 
	} // if (skinMeshComp == nullptr)
	// END OF: Skin mesh


	if (cVAOMeshManager::getInstance()->FindDrawInfoByModelName(modelInfo)) {

		glBindVertexArray(modelInfo.VAO_ID);

		glDrawElements(GL_TRIANGLES,
			modelInfo.numberOfIndices,
			GL_UNSIGNED_INT,
			0);

		glBindVertexArray(0);

	}

	return;
}

#define INSTANCE_DRAW_SIZE 100 // TODO: Gems Project 2 - Use custom shader for this....in progress
void cMeshRenderSystem::drawObjectInstances(cEntity* pCurrentMesh) {

	cShaderManager::cShaderProgram* program = cShaderManager::getInstance()->pGetShaderProgramFromFriendlyName("BasicUberShader");
	cScene* activeScene = cSceneManager::getInstance()->getActiveScene();

	glUseProgram(program->ID);

	this->bindTextures(pCurrentMesh);

	float ratio;
	int width, height;
	glm::mat4x4 matProjection = glm::mat4(1.0f);
	glm::mat4x4	matView = glm::mat4(1.0f);


	glfwGetFramebufferSize(cEngine::getInstance()->getWindow(), &width, &height);

	ratio = width / (float)height;

	matProjection = glm::perspective(0.6f,
		ratio,			// Aspect ratio
		0.1f,			// Near clipping plane
		10000.0f);		// Far clipping plane

	matView = glm::lookAt(activeScene->flyCamera.eye, activeScene->flyCamera.getAtInWorldSpace(), activeScene->flyCamera.getUpVector());

	glUniformMatrix4fv(program->getUniformID_From_Name("matView"), 1, GL_FALSE, glm::value_ptr(matView));
	glUniformMatrix4fv(program->getUniformID_From_Name("matProj"), 1, GL_FALSE, glm::value_ptr(matProjection));

	cInstancedMeshComponent* instancedComp = pCurrentMesh->getComponent<cInstancedMeshComponent>();

	std::map<std::string, cInstancedMeshComponent::sDrawInfo> instances = instancedComp->generateDrawInfo();
	std::map<std::string, cInstancedMeshComponent::sDrawInfo>::iterator it;

	for (it = instances.begin(); it != instances.end(); ++it) {

		glm::vec4 instanceColour = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
		
		if (it->first == "red") {
			
			instanceColour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		}

		if (it->first == "black") {
			
			instanceColour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		}

		if (it->first == "white") {
			
			instanceColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		}

		if (it->first == "green") {
			
			instanceColour = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		}

		if (it->first == "blue") {
			
			instanceColour = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		}

		glUniform4f(program->getUniformID_From_Name("instanceColor"), instanceColour.r, instanceColour.g, instanceColour.b, instanceColour.a);

		glm::mat4 matModel = glm::mat4x4(1.0f);
		glm::mat4 matModelInvTrans;

		//this is the root node (all the others are translated from here)
		matModel = glm::mat4(1.0f);
		glm::mat4 matTranslation = glm::translate(glm::mat4(1.0f), it->second.instances[0]->position);
		matModel = matModel * matTranslation;
		
		matModelInvTrans = glm::inverse(glm::transpose(matModel));

		cRenderMeshComponent* meshComponent = pCurrentMesh->getComponent<cRenderMeshComponent>();


		glUniformMatrix4fv(program->getUniformID_From_Name("matModel"), 1, GL_FALSE, glm::value_ptr(matModel));
		glUniformMatrix4fv(program->getUniformID_From_Name("matModelInvTrans"), 1, GL_FALSE, glm::value_ptr(matModelInvTrans));

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_CULL_FACE);

		sModelDrawInfo modelInfo;
		modelInfo.meshFileName = meshComponent->meshName;

		if (cVAOMeshManager::getInstance()->FindDrawInfoByModelName(modelInfo)) {

			//batch the drawing here
			unsigned count = 0;
			for (size_t index = 0; index < it->second.instances.size() / INSTANCE_DRAW_SIZE + 1; ++index) {

				//calculate the actual batch size
				int numInstances = INSTANCE_DRAW_SIZE;

				//get all the instanced model matrix stuff
				std::vector<glm::mat4x4> modelMatrices;
				std::vector<glm::vec4> modelPositions;

				if (it->second.instances.size() - count <= 0) continue;
				
				if (it->second.instances.size() - count < INSTANCE_DRAW_SIZE) {
					
					numInstances = it->second.instances.size() - count;
				}

				int offset = count - 1;
				if (offset < 0) {
					
					offset = 0;
				}

				count += numInstances;

				for (size_t j = 0; j < numInstances; ++j) {
					
					modelMatrices.resize(numInstances);
					modelPositions.resize(numInstances);

					matModel = glm::mat4(1.0f);

					matTranslation = glm::translate(glm::mat4(1.0f), it->second.instances[j + offset]->position);
					matModel = matModel * matTranslation;

					/*glm::mat4 matQrotation = glm::mat4(orientation);
					matModel *= matQrotation;*/

					// And now scale
					glm::mat4 matScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
					matModel = matModel * matScale;

					//save the model info
					modelMatrices[j] = matModel;
					modelPositions[j] = glm::vec4(it->second.instances[j + offset]->position, 1.0f);

					// Calculate the inverse transpose before the scaling
					matModelInvTrans = glm::inverse(glm::transpose(matModel));
				}

				glm::mat4x4* modelMatrixArray = &(modelMatrices[0]);
				glUniformMatrix4fv(program->getUniformID_From_Name("instanceMatrices"), numInstances, GL_FALSE,
					(const GLfloat*)glm::value_ptr(*modelMatrixArray));

				glUniform1f(program->getUniformID_From_Name("bIsInstanced"), GL_TRUE);
				
				glBindVertexArray(modelInfo.VAO_ID);

				glDrawElementsInstanced(GL_TRIANGLES,
					modelInfo.numberOfIndices,
					GL_UNSIGNED_INT,
					nullptr,
					numInstances);

				glBindVertexArray(0);

				glUniform1f(program->getUniformID_From_Name("bIsInstanced"), GL_FALSE);
			}
		}
	}

	return;
}

void cMeshRenderSystem::drawScene_Simple(std::vector<cEntity*> pEntities, unsigned int passNumber) {

	cShaderManager::cShaderProgram* program = cShaderManager::getInstance()->pGetShaderProgramFromFriendlyName("BasicUberShader");
	cScene* activeScene = cSceneManager::getInstance()->getActiveScene();

	std::vector<cEntity*> vec_transparentObj;

	for (unsigned int objIndex = 0; objIndex != pEntities.size(); objIndex++) {
		glm::mat4x4 matModel = glm::mat4(1.0f);

		if (pEntities[objIndex]->getComponent<cRenderMeshComponent>()->materialDiffuse.a >= 1.0f) {
			this->drawObject(pEntities[objIndex], matModel);
		}
		else {
			vec_transparentObj.push_back(pEntities[objIndex]);
		}
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	std::sort(vec_transparentObj.begin(), vec_transparentObj.end(), cPredEntityIsCloser(activeScene->flyCamera.eye));
	for (unsigned int index = 0; index != vec_transparentObj.size(); ++index) {

		glm::mat4x4 matModel = glm::mat4(1.0f);
		this->drawObject(vec_transparentObj[index], matModel);
	}

	glDisable(GL_BLEND);

	return;
}

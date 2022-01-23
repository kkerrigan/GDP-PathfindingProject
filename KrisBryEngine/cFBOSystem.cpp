/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#include "cFBOSystem.h"
#include "cMeshRenderSystem.h"

#include "cFrameBufferComponent.h"
#include "cRenderMeshComponent.h"

#include "cShaderManager.h"

#include <iostream>

cFBOSystem::cFBOSystem() {

	return;
}


cFBOSystem::~cFBOSystem() {

	return;
}

cFBOSystem* cFBOSystem::getInstance() {
	
	static cFBOSystem instance;

	return &instance;
}

void cFBOSystem::registerEntity(cEntity* entity) {

	this->entitiesToUpdate.push_back(entity);

	return;

}

void cFBOSystem::unregisterEntity(cEntity* entity) {

	// TODO: remove from vector

	return;
}

void cFBOSystem::clearFbo() {

	cShaderManager::cShaderProgram* program = cShaderManager::getInstance()->pGetShaderProgramFromFriendlyName("BasicUberShader");

	//Clear the FBO
	for (size_t index = 0; index != this->entitiesToUpdate.size(); ++index) {

		cFrameBufferComponent* currFbo = this->entitiesToUpdate[index]->getComponent<cFrameBufferComponent>();
		if (currFbo) {

			glBindFramebuffer(GL_FRAMEBUFFER, currFbo->pFbo->ID);

			currFbo->pFbo->clearBuffers(true, true);
			glUniform1f(program->getUniformID_From_Name("renderPassNumber"), 1.0f);
		}
	}

	return;
}

void cFBOSystem::update(double deltaTime) {

	cShaderManager::cShaderProgram* program = cShaderManager::getInstance()->pGetShaderProgramFromFriendlyName("BasicUberShader");

	// *****************************************
		// 2nd pass			
	// *****************************************			
	// 1. Set the Framebuffer output to the main framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);		// Points to the "regular" frame buffer
	
	glEnable(GL_DEPTH);		// Enables the KEEPING of the depth information
	glEnable(GL_DEPTH_TEST);	// When drawing, checked the existing depth
	glEnable(GL_CULL_FACE);	// Discared "back facing" triangles

	// 2. Clear everything **ON THE MAIN FRAME BUFFER** 
	// (NOT the offscreen buffer)
	if (this->entitiesToUpdate.size() != 0) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	for (size_t index = 0; index != this->entitiesToUpdate.size(); ++index) {

		cFrameBufferComponent* currFbo = this->entitiesToUpdate[index]->getComponent<cFrameBufferComponent>();
		if (currFbo) {

			// 3. Bind 1 texture (what we drew)
			cRenderMeshComponent* meshComp = this->entitiesToUpdate[index]->getComponent<cRenderMeshComponent>();
			meshComp->bIsVisible = true;
			meshComp->bDontLight = true;
			meshComp->bUseVertexColour = false;
			meshComp->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));

			// Tell the shader this is the 2nd pass...
			// This will run a very simple shader, which
			//  does NOT lighting, and only samples from a single texture
			//  (for now: soon there will be multiple textures)
			glUniform1f(program->getUniformID_From_Name("renderPassNumber"), 2.0f);

			// TODO: FBO - ratio and camera full screen quad

			// 4. Draw a single quad
			GLint program = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");
			glm::mat4 matModel = glm::mat4(1.0f);

			cMeshRenderSystem::getInstance()->drawObject(this->entitiesToUpdate[index], matModel);

			meshComp->bIsVisible = false;
		}
	}

	return;
}

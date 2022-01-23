#include <cShaderManager.h>
#include <iostream>

bool loadShaders() {
	
	// load shaders
	// BasicUberShader
	cShaderManager* pShaderManager = cShaderManager::getInstance();
	pShaderManager->setBasePath("./assets/shaders/");

	cShaderManager::cShader vertexShader;
	cShaderManager::cShader fragmentShader;

	vertexShader.fileName = "vertex01.glsl";
	vertexShader.shaderType = cShaderManager::cShader::VERTEX_SHADER;

	fragmentShader.fileName = "fragment01.glsl";
	fragmentShader.shaderType = cShaderManager::cShader::FRAGMENT_SHADER;

	if (!pShaderManager->createProgramFromFile("BasicUberShader", vertexShader, fragmentShader)) {
		
		std::cout << "OH NO! Compile error" << std::endl;
		std::cout << pShaderManager->getLastError() << std::endl;
		
		return false;

	} else {
		
		// BasicUberShader Uniforms
		cShaderManager::cShaderProgram* pSP = pShaderManager->pGetShaderProgramFromFriendlyName("BasicUberShader");

		// Matrix uniforms
		pSP->LoadUniformLocation("matModel");
		pSP->LoadUniformLocation("matView");
		pSP->LoadUniformLocation("matProj");
		pSP->LoadUniformLocation("matModelInvTrans");
		pSP->LoadUniformLocation("eyeLocation");

		// Colour uniforms
		pSP->LoadUniformLocation("objectDiffuse");
		pSP->LoadUniformLocation("objectSpecular");
		pSP->LoadUniformLocation("wholeObjectAlphaTransparency");

		// Boolean uniforms
		pSP->LoadUniformLocation("useVertexColour");
		pSP->LoadUniformLocation("bDontUseLighting");

		// Skybox uniforms
		pSP->LoadUniformLocation("textureSkyBox");
		pSP->LoadUniformLocation("useSkyBoxTexture");

		// Particle System
		pSP->LoadUniformLocation("bIsParticleImposter");
		pSP->LoadUniformLocation("ParticleImposterAlphaOverride");
		pSP->LoadUniformLocation("ParticleImposterBlackThreshold");

		// Skinned mesh uniforms
		pSP->LoadUniformLocation("bIsASkinnedMesh");
		pSP->LoadUniformLocation("numBonesUsed");
		pSP->LoadUniformLocation("bones");

		// Texture uniforms
		pSP->LoadUniformLocation("texture00");
		pSP->LoadUniformLocation("texture01");
		pSP->LoadUniformLocation("texture02");
		pSP->LoadUniformLocation("texture03");
		pSP->LoadUniformLocation("texture04");
		pSP->LoadUniformLocation("texture05");
		pSP->LoadUniformLocation("texture06");
		pSP->LoadUniformLocation("texture07");
		pSP->LoadUniformLocation("texBlendWeights[0]");
		pSP->LoadUniformLocation("texBlendWeights[1]");
		pSP->LoadUniformLocation("texPass1OutputTexture");

		// Instance Drawing
		// TODO: Gems Project 2 - Get the custom shader working for this instead
		if (!pSP->LoadUniformLocation("instanceColor")) {

			std::cout << "Did not load instanceColor uniform" << std::endl;
		}

		if (!pSP->LoadUniformLocation("instanceMatrices")) {

			std::cout << "Did not load instancedModelMat uniform" << std::endl;
		}

		if (!pSP->LoadUniformLocation("bIsInstanced")) {

			std::cout << "Did not load the bIsInstanced uniform" << std::endl;
		}
	}

	// Freetype Shaders
	cShaderManager::cShader vertexShaderFT;
	cShaderManager::cShader fragmentShaderFT;

	vertexShaderFT.fileName = "freetype_v.glsl";
	vertexShaderFT.shaderType = cShaderManager::cShader::VERTEX_SHADER;

	fragmentShaderFT.fileName = "freetype_f.glsl";
	fragmentShaderFT.shaderType = cShaderManager::cShader::FRAGMENT_SHADER;

	if (!pShaderManager->createProgramFromFile("FreeTypeShader", vertexShaderFT, fragmentShaderFT)) {

		std::cout << "OH NO! Compile error" << std::endl;
		std::cout << pShaderManager->getLastError() << std::endl;
		return false;
	}
	else {

		// FreeType uniforms
		cShaderManager::cShaderProgram* pSP = pShaderManager->pGetShaderProgramFromFriendlyName("FreeTypeShader");
		pSP->LoadUniformLocation("tex");
		pSP->LoadUniformLocation("color");
		pSP->LoadAtrributeLocation("coord");

	}

	// Instance drawing shaders
	/*cShaderManager::cShader vertexShaderInstance;
	cShaderManager::cShader fragmentShaderInstance;

	vertexShaderInstance.fileName = "vertexInstance.glsl";
	vertexShaderInstance.shaderType = cShaderManager::cShader::VERTEX_SHADER;

	fragmentShaderInstance.fileName = "fragmentInstance.glsl";
	fragmentShaderInstance.shaderType = cShaderManager::cShader::FRAGMENT_SHADER;

	if (!pShaderManager->createProgramFromFile("InstanceShader", vertexShaderInstance, fragmentShaderInstance)) {

		std::cout << "OH NO! Compile error" << std::endl;
		std::cout << pShaderManager->getLastError() << std::endl;
		return false;
	}
	else {

		cShaderManager::cShaderProgram* pSP = pShaderManager->pGetShaderProgramFromFriendlyName("InstanceShader");
		pSP->LoadUniformLocation("projection");
		pSP->LoadUniformLocation("view");
		pSP->LoadUniformLocation("objectDiffuse");
		pSP->LoadUniformLocation("texture00");
		pSP->LoadUniformLocation("texBlendWeights[0]");
		pSP->LoadUniformLocation("texBlendWeights[1]");
	}*/

	std::cout << "Compiled shaders OK!" << std::endl;
	return true;
}
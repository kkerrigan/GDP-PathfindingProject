#include  "cEngine.h"

#include "cEulerMovementSystem.h"
#include "cMeshRenderSystem.h"
#include "cColliderSystem.h"
#include "cParticleSystem.h"
#include "cScriptingCommandSystem.h"
#include "cBehaviourSystem.h"
#include "cFBOSystem.h"

#include "cDebugRenderer.h"
#include "cShaderManager.h"
#include "cLightManager.h"
#include "sLight.h"
#include "cSoundManager.h"

// c'tor
cEngine::cEngine() : pWindow(nullptr), pFuncUpdateCallback(nullptr), pFuncKeyboardCallbackAsync(nullptr), pFuncMouseCallbackAsync(nullptr) {}

// d'tor
cEngine::~cEngine() {
	
	this->terminate();

	return;
}

cEngine* cEngine::getInstance() {
	static cEngine instance;

	return &instance;
}

GLFWwindow* cEngine::getWindow() {
	return this->pWindow;
}

sConfigSettings* cEngine::getConfigSettings() {

	return this->config;
}

/*
*	Initialize the engine (things like glfw, render window and glad)
*
*	returns true on success and false on failure
*/
bool cEngine::initialize(sConfigSettings* pConfigSettings) {

	// check if we have initialized the engine already
	if (this->pWindow != nullptr) {
		return false;
	}

	this->config = pConfigSettings;

	// initialize glfw
	if (!glfwInit()) {
		return false;
	}

	// set some hints for the driver
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	// create a window
	this->pWindow = glfwCreateWindow(pConfigSettings->width, pConfigSettings->height, pConfigSettings->title.c_str(), NULL, NULL);
	if (!this->pWindow)
	{
		glfwTerminate();
		return false;
	}

	// make the window the current context
	glfwMakeContextCurrent(this->pWindow);
	glfwSwapInterval(1);

	// get all the openGL function now that we have a window
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	// Load the physics DLL 
	if (!this->mPhysicsDll.loadDll(pConfigSettings->physicsDll)) {
		std::cout << "Failed to load DLL" << std::endl;
		return false;
	}

	// Create the factory function in the DLL
	const nPhysics::fCreatePhysicsFactory factoryFunction = this->mPhysicsDll.getProcAddress<nPhysics::fCreatePhysicsFactory>("createPhysicsFactory");
	if (factoryFunction == nullptr) {
		std::cout << "Problem getting Proc Address for createPhysicsFactory" << std::endl;
		return false;
	}

	// Create physics objects
	this->mPhysicsFactory = factoryFunction();
	if (this->mPhysicsFactory == nullptr) {
		std::cout << "Problem creating Physics Factory" << std::endl;
		return false;
	}

	this->mPhysicsWorld = this->mPhysicsFactory->createPhysicsWorld();
	if (this->mPhysicsWorld == nullptr) {
		std::cout << "Problem creating Physics World" << std::endl;
		return false;
	}

	return true;
}

/*
*	Enter the game loop 
*
*/
void cEngine::startGameLoop() {
	
	int program = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");
	
	double lastFrameTime = glfwGetTime();
	while (!glfwWindowShouldClose(this->pWindow)) {

		// keep track of delta time
		double nowTime = glfwGetTime();
		double deltaTime = nowTime - lastFrameTime;
		lastFrameTime = nowTime;

		if (deltaTime > 0.1f) {
			deltaTime = 0.1f;
		}

		// clear the backbuffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// push lights to gpu
		cLightManager* lightManager = cLightManager::getInstance();
		lightManager->writeLightsToShader();

		// process the keys async
		if (this->pFuncKeyboardCallbackAsync != nullptr) {
			this->pFuncKeyboardCallbackAsync(this->pWindow, deltaTime);
		}

		// get mouse input
		if (this->pFuncMouseCallbackAsync != nullptr) {
			this->pFuncMouseCallbackAsync(this->pWindow);
		}

		// Physics update
		if (this->mPhysicsWorld != nullptr) {
			this->mPhysicsWorld->update(deltaTime);
		}

		// Clear FBOs
		cFBOSystem::getInstance()->clearFbo();

		// run all the component systems
		cEulerMovementSystem::getInstance()->update(deltaTime);
		cColliderSystem::getInstance()->update(deltaTime);
		cMeshRenderSystem::getInstance()->update(deltaTime);
		cBehaviourSystem::getInstance()->update(deltaTime);
		cParticleSystem::getInstance()->update(deltaTime);

		// Scripted commands
		cScriptingCommandSystem::getInstance()->update(deltaTime);

		//update 3d sounds
		cSoundManager::getInstance()->update();

		// if the update callback is set call it
		// NOTE: Moved because of FreeType
		if (this->pFuncUpdateCallback != nullptr) {
			this->pFuncUpdateCallback(deltaTime);
		}

		// present the back buffer
		glfwSwapBuffers(this->pWindow);

		// allow user input
		glfwPollEvents();
	}

	return;
}

/*
*	Clean-up the engine (glfw..)
*
*/
void cEngine::terminate() {
	
	glfwDestroyWindow(this->pWindow);
	glfwTerminate();

	this->mPhysicsDll.unloadDll();

	return;
}

void cEngine::setUpdateCallback(update_callback_function_def func) {
	this->pFuncUpdateCallback = func;

	return;
}

bool cEngine::setKeyboardCallback(keyboard_callback_function_def func) {
	if (this->pWindow == nullptr) {
		return false;
	}

	glfwSetKeyCallback(this->pWindow, func);

	return true;
}

void cEngine::setKeyboardCallbackAsync(keyboard_callback_async_function_def func) {
	this->pFuncKeyboardCallbackAsync = func;

	return;
}

void cEngine::setMouseCallbackAsync(mouse_callback_async_function_def func) {
	this->pFuncMouseCallbackAsync = func;

	return;
}

void cEngine::setMouseCursorPositionCallback(mouse_cursor_pos_function_def func) {
	glfwSetCursorPosCallback(this->pWindow, func);

	return;
}

void cEngine::setMouseButtonCallback(mouse_button_callback_function_def func) {
	glfwSetMouseButtonCallback(this->pWindow, func);

	return;
}

void cEngine::setMouseEnterCallback(mouse_cursor_enter_function_def func) {
	glfwSetCursorEnterCallback(this->pWindow, func);

	return;
}

void cEngine::setMouseScrollCallback(mouse_scroll_callback_function_def func) {
	glfwSetScrollCallback(this->pWindow, func);

	return;
}

nPhysics::iPhysicsFactory * cEngine::getPhysicsFactory() {
	
	return this->mPhysicsFactory;
}

nPhysics::iPhysicsWorld * cEngine::getPhysicsWorld() {
	
	return this->mPhysicsWorld;
}
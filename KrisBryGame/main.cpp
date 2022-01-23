#include <iostream>
#include <cEngine.h>
#include <cConsole.h>
#include <cShaderManager.h>
#include <cVAOMeshManager.h>
#include <cEntityBuilder.h>
#include <cDebugRenderer.h>
#include <cSoundManager.h>
#include <cSerialization.h>
#include <cFreeTypeRenderer.h>

// forward declare some functions
bool loadShaders();
void loadTextures();

void updateCallback(double deltaTime);

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void keyboardCallbackASYNC(GLFWwindow* window, float deltaTime);

void mouseCallbackAsync(GLFWwindow* window);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_enter_callback(GLFWwindow* window, int entered);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void loadModels();
void loadScene();
void loadLights();
//void loadTerrainAABBHierarchy();

int main() {

	cConsole console;

	// initialize the engine and window
	cEngine* pEngine = cEngine::getInstance();
	sConfigSettings configSettings;

	// Physics Project 3 addition....load config file 
	if (!cSerialization::deserializeConfigSettings("config.json", configSettings)) {
		std::cout << "Failed to load configuration settings" << std::endl;
		exit(EXIT_FAILURE);
	}

	if (!pEngine->initialize(&configSettings)) {
		std::cout << "Failed to initialize engine." << std::endl;
		exit(EXIT_FAILURE);
	}

	if (!loadShaders()) {
		std::cout << "Failed to load shaders." << std::endl;
		exit(EXIT_FAILURE);
	}

	cFreeTypeRenderer* text_renderer = cFreeTypeRenderer::getInstance();
	text_renderer->initText(16);

	// Set up SOund Manager
	cSoundManager* pSoundManager = cSoundManager::getInstance();
	pSoundManager->initialize();
	pSoundManager->setBasePath("./assets/sounds/");

	// set lots of callbacks
	pEngine->setUpdateCallback(updateCallback);
	pEngine->setKeyboardCallback(keyboardCallback);
	pEngine->setKeyboardCallbackAsync(keyboardCallbackASYNC);
	pEngine->setMouseCallbackAsync(mouseCallbackAsync);
	pEngine->setMouseCursorPositionCallback(cursor_position_callback);
	pEngine->setMouseButtonCallback(mouse_button_callback);
	pEngine->setMouseEnterCallback(cursor_enter_callback);
	pEngine->setMouseScrollCallback(scroll_callback);

	// setup debug renderer
	cDebugRenderer::getInstance()->initialize();

	//load textures
	loadTextures();

	//load model files
	loadModels();

	// load the entities
	loadScene();

	// load the terrain aabb Hierarchy (for broad phase collision detection)
	//loadTerrainAABBHierarchy();

	// load lights
	//loadLights();

	pEngine->getPhysicsWorld()->setGravity(glm::vec3(0.0f, -9.81f, 0.0f));

	// start the game loop (hang on..!)
	pEngine->startGameLoop();

	cSoundManager::getInstance()->shutDown();

	return 0;
}

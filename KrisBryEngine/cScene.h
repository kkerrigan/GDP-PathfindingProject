/*
* Mediator for the scene
*/

#ifndef _cScene_HG_
#define _cScene_HG_

#include <gameMath.h>
#include <vector>
#include <string>

#include "cEntity.h"
#include "sLight.h"
#include "iInputCommand.h"
#include "cFlyCamera.h"

#include "cEntityBuilder.h"
#include "cLightManager.h"
#include "cSoundManager.h"
#include "c3dSoundComponent.h"

#include "cMazeMaker.h"
#include "cPathFinding.h"

class cScene {
public:
	cScene();
	~cScene();

	int selectedEntity;
	int selectedLight;
	int selectedChannelGroup;

	std::string sceneName;

	cFlyCamera flyCamera;
	bool isCursorInWindow;

	//TODO: remove old camera
	glm::vec3 cameraPos;
	glm::vec3 cameraLookAt;
	glm::vec3 cameraUp;

	// Scene modifiers
	void setSceneName(std::string name);
	void updateWindowTitle(GLFWwindow* window);
	bool getIsScenePaused();
	void setIsScenePaused(bool val);
	void handleCommand(iInputCommand* command);

	// Scene cameras
	void setCameraPosition(glm::vec3 pos);
	void setCameraLookAt(glm::vec3 lookAt);
	
	// Scene entities
	void addEntityToScene(cEntity* entity);
	void removeEntityFromScene(cEntity* entiy);
	cEntity* getSelectedEntity();
	cEntity* findObjectByFriendlyName(std::string theNameToFind);
	cEntity* findObjectById(int id);
	size_t getNumberOfEntites();
	std::vector<cEntity*>& getEntities();

	// Rigid bodies
	void changeSelectedRigidBody(bool next = true);

	// Scene lights
	void addLightToScene(sLight* light);
	void removeLightFromScene(sLight* light);
	sLight* getSelectedLight();
	sLight* getLightAt(unsigned int index);
	size_t getNumberOfLights();
	std::vector<sLight*> getLights();

	// Scene non-3D sounds
	void addSoundToScene(cSoundManager::sSoundInfo* soundInfo);
	cSoundManager::sSoundInfo* getSoundInfo(unsigned index);
	std::vector<cSoundManager::sSoundInfo*> getSounds();

	// Debug Functions
	bool bIsLightDebug;
	bool bIsRenderDebug;
	bool bDisplayAABBS;
	bool bDisplayCamInfo;
	void setLightDebugSphere(cEntity* pSphere);

	// Gems Midterm Q1
	cMazeMaker sceneMaze;
	void createMaze(int mazeWidth, int mazeHeight);
	std::vector<glm::vec3> emptySquares;

	// Path Finding
	cPathFinding* pathfinding;

private:
	std::vector<cEntity*> entities;
	std::vector<sLight*> lights;
	std::vector<cSoundManager::sSoundInfo*> sounds;
	bool isScenePaused;

	// Debug light constants
	const float ACCURACY_OF_DISTANCE = 0.01f;
	const float INFINITE_DISTANCE = 1000.0f;
};

#endif

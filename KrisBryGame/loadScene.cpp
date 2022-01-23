#include <Global.h>
#include <cVAOMeshManager.h>
#include <cEntityBuilder.h>
#include <cSceneManager.h>
#include <cLightManager.h>
#include <cSoundManager.h>
#include <cLuaBrain.h>
#include <cScriptingCommandSystem.h>
#include <cColliderSystem.h>
#include <cSerialization.h>
#include <cMapLoader.h>
#include <cFrameBufferComponent.h>
#include <cRigidBodyComponent.h>
#include <cThreadComponent.h>
#include <cInstancedMeshComponent.h>
#include <fstream>

#include <cBehaviourSystem.h>
#include <cBehaviourComponent.h>
#include <cIdleBehaviour.h>
#include <cFormationBehaviour.h>
#include <cCrowdFollowBehaviour.h>
#include <cFlockBehaviour.h>

#include <cPathFinding.h>
#include <PathingStructs.h>
#include <cPathFindingBehaviour.h>
#include <cGathererBehaviour.h>

cEntity* pSphere = nullptr;
cEntity* pCube = nullptr;

sLight* pMovingSpotLight = nullptr;

extern std::map<std::string, cCommandGroup*> commandGroups;
extern std::map<std::string, iCommand*> commands;
extern cAABBTerrianBroadPhaseHierarchy g_AABBTerrianHierarchy;
cFormationBehaviour* pFormation;
cCrowdFollowBehaviour* pCrowdFollow;

cInstancedMeshComponent* instanceRenderer;

void set_rand_position(cTransformComponent* transform_component) {
	
	float x = rand() % 200 - 100.0f;
	float z = rand() % 200 - 100.0f;

	transform_component->setPosition(glm::vec3(x, 0.0f, z));
}

void loadScene() {

	// create a scene for the entities
	cSceneManager* pSceneManager = cSceneManager::getInstance();
	cScene* pSandboxScene = pSceneManager->createScene("sandbox.json");
	pSceneManager->setActiveScene("sandbox.json");

	//initialize the light manager before loading them
	cLightManager::getInstance()->initializeLights();

	pSphere = cEntityBuilder::getInstance()->createEntity(0);
	cRenderMeshComponent* sphereMesh = pSphere->getComponent<cRenderMeshComponent>();
	sphereMesh->bIsVisible = false;
	sphereMesh->friendlyName = "Sphere";
	sphereMesh->bIsWireFrame = false;

	pCube = cEntityBuilder::getInstance()->createEntity(3);
	cRenderMeshComponent* cubeMesh = pCube->getComponent<cRenderMeshComponent>();
	cubeMesh->bIsVisible = false;
	cubeMesh->friendlyName = "Cube";
	cubeMesh->bIsWireFrame = true;

	cColliderSystem::getInstance()->pColliderSphere = pSphere;
	cColliderSystem::getInstance()->pAABBCube = pCube;

	//load from the file
	cSerialization::deserializeSceneCamera("cameras.json");
	cSerialization::deserializeSceneLights("lights.json");
	cSerialization::deserializeSceneSounds("sounds.json");
	cSerialization::deserializeSceneEntities("entities.json");

	cEntityBuilder* pBuilder = cEntityBuilder::getInstance();

	// Load bitmap level
	cMapLoader map;
	map.initialize();
	unsigned char* mapData = map.loadMapData("resourceMap.bmp");
	//map.printMap();

	int mapWidth = map.getMapWidth();
	int mapHeight = map.getMapHeight();

	cEntity* instancedCube = pBuilder->createEntity(3);
	pSandboxScene->addEntityToScene(instancedCube);
	
	pSandboxScene->pathfinding = new cPathFinding();
	glm::vec3 nodeOffset = glm::vec3(mapHeight - 1.0f, 0.0f, -mapWidth + 1.0f);

	// First create the nodes
	int index = 0;
	for (size_t x = 0; x < mapHeight; x++) {
		
		for (size_t y = 0; y < mapWidth; y++) {
			
			sPathFindingNode* node = new sPathFindingNode();

			node->position = glm::vec3(nodeOffset.x + (x*-2.0f), 0.0f, nodeOffset.z + (y*2.0f));

			float r = mapData[index++];
			float g = mapData[index++];
			float b = mapData[index++];

			if (map.isValidTile(r, g, b, cMapLoader::eTileType::TRAVERSABLE)) {
				
				node->bIsTraversable = true;
			}

			if (map.isValidTile(r, g, b, cMapLoader::eTileType::NON_TRAVERSABLE)) {
				
				node->bIsTraversable = false;
			}

			if (map.isValidTile(r, g, b, cMapLoader::eTileType::START)) {
				
				node->bIsTraversable = true;
				node->bIsGatherer = true;

				{
					// Create a Dalek to follow that path
					cEntity* player = pBuilder->createEntity(7);
					cTransformComponent* playerTrans = player->getComponent<cTransformComponent>();
					cRenderMeshComponent* playerMesh = player->getComponent<cRenderMeshComponent>();

					playerTrans->setPosition(node->position);
					playerMesh->friendlyName = "Player";

					cBehaviourComponent* playerBehaviours = player->getComponent<cBehaviourComponent>();
					cGathererBehaviour* gatherBehaviour = playerBehaviours->addBehaviour<cGathererBehaviour>();

					gatherBehaviour->setActiveScene(pSandboxScene);
					gatherBehaviour->setResourceWaitTime(7.0f);
					gatherBehaviour->setHomeBaseWaitTime(2.0f);
					gatherBehaviour->setActiveBehaviour(new cIdleBehaviour(2.0f));

					cBehaviourSystem::getInstance()->registerEntity(player);
					pSandboxScene->addEntityToScene(player);
				}
			}

			if (map.isValidTile(r, g, b, cMapLoader::eTileType::RESOURCE)) {
				
				node->bIsTraversable = true;
				node->bIsResource = true;
			}

			if (map.isValidTile(r, g, b, cMapLoader::eTileType::HOME_BASE)) {
				
				node->bIsTraversable = true;
				node->bIsHomeBase = true;
			}

			pSandboxScene->pathfinding->path.pathNodes.push_back(node);
		}
	}

	// Now find all the neighbours
	pSandboxScene->pathfinding->findNodeNeighbors(mapWidth, mapHeight);

	// Finally, draw the map
	instanceRenderer = instancedCube->getComponent<cInstancedMeshComponent>();
	instanceRenderer->updateInstances(pSandboxScene->pathfinding->path.pathNodes);

	// Scale the terrian to the map size
	cEntity* plane = pSandboxScene->findObjectByFriendlyName("BottomPlane");
	cTransformComponent* planeTrans = plane->getComponent<cTransformComponent>();
	planeTrans->scale = glm::vec3(mapHeight * 2.0f / 256.0f, 1.0f, mapWidth * 2.0f / 256.0f);

	return;
}
/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#include <rapidjson/prettywriter.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/istreamwrapper.h>

#include <fstream>

#include "cSerialization.h"

#include "cEngine.h"
#include "cMeshRenderSystem.h"

#include "cSceneManager.h"
#include "cEulerMovementComponent.h"
#include "cRigidBodyComponent.h"
#include "cClothComponent.h"
#include "cParticleComponent.h"

bool cSerialization::serializeSceneCamera(std::string fileName) {

	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

	// Get active scene
	cScene* pScene = cSceneManager::getInstance()->getActiveScene();

	writer.StartObject();

	writer.String("sceneName");
	writer.String(pScene->sceneName.c_str());
	writer.String("selectedEntity");
	writer.Int(pScene->selectedEntity);
	writer.String("selectedLight");
	writer.Int(pScene->selectedLight);

	writer.String(("camera"));
	writer.StartObject();

	writer.String("position");
	writer.StartObject();
	writer.String("x");
	writer.Double(pScene->flyCamera.eye.x);
	writer.String("y");
	writer.Double(pScene->flyCamera.eye.y);
	writer.String("z");
	writer.Double(pScene->flyCamera.eye.z);
	writer.EndObject();

	writer.String("orientation");
	writer.StartObject();
	writer.String("x");
	writer.Double(pScene->flyCamera.getQOrientation().x);
	writer.String("y");
	writer.Double(pScene->flyCamera.getQOrientation().y);
	writer.String("z");
	writer.Double(pScene->flyCamera.getQOrientation().z);
	writer.String("w");
	writer.Double(pScene->flyCamera.getQOrientation().w);
	writer.EndObject();

	writer.String("up");
	writer.StartObject();
	writer.String("x");
	writer.Double(pScene->flyCamera.getUpVector().x);
	writer.String("y");
	writer.Double(pScene->flyCamera.getUpVector().y);
	writer.String("z");
	writer.Double(pScene->flyCamera.getUpVector().z);
	writer.EndObject();

	writer.EndObject();
	writer.EndObject();

	// Write the JSON object to a file
	std::ofstream file("./assets/maps/" + fileName);

	if (!file.is_open()) {
		return false;
	}
	file << buffer.GetString();

	return true;
}

bool cSerialization::deserializeSceneCamera(std::string fileName) {

	std::ifstream ifs("./assets/maps/" + fileName);

	if (!ifs.is_open()) {
		return false;
	}

	rapidjson::IStreamWrapper isw(ifs);
	rapidjson::Document document;
	document.ParseStream(isw);

	// Get active scene
	cScene* pScene = cSceneManager::getInstance()->getActiveScene();

	//scene name
	assert(document.HasMember("sceneName"));
	assert(document["sceneName"].IsString());
	pScene->sceneName = document["sceneName"].GetString();

	//selected entity
	assert(document.HasMember("selectedEntity"));
	assert(document["selectedEntity"].IsInt());
	pScene->selectedEntity = document["selectedEntity"].GetInt();

	//selected light
	assert(document.HasMember("selectedLight"));
	assert(document["selectedLight"].IsInt());
	pScene->selectedLight = document["selectedLight"].GetInt();

	//camera
	assert(document.HasMember("camera"));
	assert(document["camera"].IsObject());

	// position
	assert(document["camera"].HasMember("position"));
	assert(document["camera"]["position"].IsObject());

	assert(document["camera"]["position"].HasMember("x"));
	assert(document["camera"]["position"]["x"].IsDouble());
	pScene->flyCamera.eye.x = (float)document["camera"]["position"]["x"].GetDouble();

	assert(document["camera"]["position"].HasMember("y"));
	assert(document["camera"]["position"]["y"].IsDouble());
	pScene->flyCamera.eye.y = (float)document["camera"]["position"]["y"].GetDouble();

	assert(document["camera"]["position"].HasMember("z"));
	assert(document["camera"]["position"]["z"].IsDouble());
	pScene->flyCamera.eye.z = (float)document["camera"]["position"]["z"].GetDouble();

	// look at
	glm::quat orientation;
	assert(document["camera"].HasMember("orientation"));
	assert(document["camera"]["orientation"].IsObject());

	assert(document["camera"]["orientation"].HasMember("x"));
	assert(document["camera"]["orientation"]["x"].IsDouble());
	orientation.x = (float)document["camera"]["orientation"]["x"].GetDouble();

	assert(document["camera"]["orientation"].HasMember("y"));
	assert(document["camera"]["orientation"]["y"].IsDouble());
	orientation.y = (float)document["camera"]["orientation"]["y"].GetDouble();

	assert(document["camera"]["orientation"].HasMember("z"));
	assert(document["camera"]["orientation"]["z"].IsDouble());
	orientation.z = (float)document["camera"]["orientation"]["z"].GetDouble();

	assert(document["camera"]["orientation"].HasMember("w"));
	assert(document["camera"]["orientation"]["w"].IsDouble());
	orientation.w = (float)document["camera"]["orientation"]["w"].GetDouble();

	pScene->flyCamera.adjMeshOrientationQ(orientation);


	// camera up
	assert(document["camera"].HasMember("up"));
	assert(document["camera"]["up"].IsObject());

	assert(document["camera"]["up"].HasMember("x"));
	assert(document["camera"]["up"]["x"].IsDouble());
	pScene->cameraUp.x = (float)document["camera"]["up"]["x"].GetDouble();

	assert(document["camera"]["up"].HasMember("y"));
	assert(document["camera"]["up"]["y"].IsDouble());
	pScene->cameraUp.y = (float)document["camera"]["up"]["y"].GetDouble();

	assert(document["camera"]["up"].HasMember("z"));
	assert(document["camera"]["up"]["z"].IsDouble());
	pScene->cameraUp.z = (float)document["camera"]["up"]["z"].GetDouble();

	return true;
}

bool cSerialization::serializeSceneLights(std::string fileName) {

	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

	std::vector<sLight*> lights = cSceneManager::getInstance()->getActiveScene()->getLights();

	writer.StartObject();

	writer.String(("lights"));
	writer.StartArray();
	for (std::vector<sLight*>::iterator light = lights.begin(); light != lights.end(); ++light) {
		(*light)->serializeComponent(writer);
	}
	writer.EndArray();

	writer.EndObject();

	// Write the JSON object to a file
	std::ofstream file("./assets/maps/" + fileName);

	if (!file.is_open()) {
		return false;
	}
	file << buffer.GetString();

	return true;
}

bool cSerialization::deserializeSceneLights(std::string fileName) {

	std::ifstream ifs("./assets/maps/" + fileName);

	if (!ifs.is_open()) {
		return false;
	}

	rapidjson::IStreamWrapper isw(ifs);
	rapidjson::Document document;
	document.ParseStream(isw);

	assert(document.HasMember("lights"));
	assert(document["lights"].IsArray());
	rapidjson::GenericArray < false, rapidjson::Value::ValueType> lights = (document["lights"].GetArray());

	// Get active scene
	cScene* pScene = cSceneManager::getInstance()->getActiveScene();

	unsigned int lightCount = 0;
	cLightManager* pLightManager = cLightManager::getInstance();
	for (rapidjson::Value::ConstValueIterator itr = lights.Begin(); itr != lights.End(); ++itr) { // Ok

		sLight* light = pLightManager->getLightByIndex(lightCount);

		//position
		assert(itr->HasMember("position"));
		assert((*itr)["position"].IsObject());

		assert((*itr)["position"].HasMember("x"));
		assert((*itr)["position"]["x"].IsDouble());
		light->position.x = (float)(*itr)["position"]["x"].GetDouble();

		assert((*itr)["position"].HasMember("y"));
		assert((*itr)["position"]["y"].IsDouble());
		light->position.y = (float)(*itr)["position"]["y"].GetDouble();

		assert((*itr)["position"].HasMember("z"));
		assert((*itr)["position"]["z"].IsDouble());
		light->position.z = (float)(*itr)["position"]["z"].GetDouble();

		//specular
		assert(itr->HasMember("specular"));
		assert((*itr)["specular"].IsObject());

		assert((*itr)["specular"].HasMember("r"));
		assert((*itr)["specular"]["r"].IsDouble());
		light->specular.x = (float)(*itr)["specular"]["r"].GetDouble();

		assert((*itr)["specular"].HasMember("g"));
		assert((*itr)["specular"]["g"].IsDouble());
		light->specular.y = (float)(*itr)["specular"]["g"].GetDouble();

		assert((*itr)["specular"].HasMember("b"));
		assert((*itr)["specular"]["b"].IsDouble());
		light->specular.z = (float)(*itr)["specular"]["b"].GetDouble();

		assert((*itr)["specular"].HasMember("a"));
		assert((*itr)["specular"]["a"].IsDouble());
		light->specular.w = (float)(*itr)["specular"]["a"].GetDouble();

		//diffuse
		assert(itr->HasMember("diffuse"));
		assert((*itr)["diffuse"].IsObject());

		assert((*itr)["diffuse"].HasMember("r"));
		assert((*itr)["diffuse"]["r"].IsDouble());
		light->diffuse.x = (float)(*itr)["diffuse"]["r"].GetDouble();

		assert((*itr)["diffuse"].HasMember("g"));
		assert((*itr)["diffuse"]["g"].IsDouble());
		light->diffuse.y = (float)(*itr)["diffuse"]["g"].GetDouble();

		assert((*itr)["diffuse"].HasMember("b"));
		assert((*itr)["diffuse"]["b"].IsDouble());
		light->diffuse.z = (float)(*itr)["diffuse"]["b"].GetDouble();

		assert((*itr)["diffuse"].HasMember("a"));
		assert((*itr)["diffuse"]["a"].IsDouble());
		light->diffuse.w = (float)(*itr)["diffuse"]["a"].GetDouble();

		//attenuation
		assert(itr->HasMember("attenuation"));
		assert((*itr)["attenuation"].IsObject());

		assert((*itr)["attenuation"].HasMember("constant"));
		assert((*itr)["attenuation"]["constant"].IsDouble());
		light->atten.x = (float)(*itr)["attenuation"]["constant"].GetDouble();

		assert((*itr)["attenuation"].HasMember("linear"));
		assert((*itr)["attenuation"]["linear"].IsDouble());
		light->atten.y = (float)(*itr)["attenuation"]["linear"].GetDouble();

		assert((*itr)["attenuation"].HasMember("quadratic"));
		assert((*itr)["attenuation"]["quadratic"].IsDouble());
		light->atten.z = (float)(*itr)["attenuation"]["quadratic"].GetDouble();

		assert((*itr)["attenuation"].HasMember("cutoffDist"));
		assert((*itr)["attenuation"]["cutoffDist"].IsDouble());
		light->atten.w = (float)(*itr)["attenuation"]["cutoffDist"].GetDouble();

		//direction
		assert(itr->HasMember("direction"));
		assert((*itr)["direction"].IsObject());

		assert((*itr)["direction"].HasMember("x"));
		assert((*itr)["direction"]["x"].IsDouble());
		light->direction.x = (float)(*itr)["direction"]["x"].GetDouble();

		assert((*itr)["direction"].HasMember("y"));
		assert((*itr)["direction"]["y"].IsDouble());
		light->direction.y = (float)(*itr)["direction"]["y"].GetDouble();

		assert((*itr)["direction"].HasMember("z"));
		assert((*itr)["direction"]["z"].IsDouble());
		light->direction.z = (float)(*itr)["direction"]["z"].GetDouble();

		assert((*itr)["direction"].HasMember("w"));
		assert((*itr)["direction"]["w"].IsDouble());
		light->direction.w = (float)(*itr)["direction"]["w"].GetDouble();

		//param1
		assert(itr->HasMember("param1"));
		assert((*itr)["param1"].IsObject());

		assert((*itr)["param1"].HasMember("lightType"));
		assert((*itr)["param1"]["lightType"].IsDouble());
		light->param1.x = (float)(*itr)["param1"]["lightType"].GetDouble();

		assert((*itr)["param1"].HasMember("innerAngle"));
		assert((*itr)["param1"]["innerAngle"].IsDouble());
		light->param1.y = (float)(*itr)["param1"]["innerAngle"].GetDouble();

		assert((*itr)["param1"].HasMember("outerAngle"));
		assert((*itr)["param1"]["outerAngle"].IsDouble());
		light->param1.z = (float)(*itr)["param1"]["outerAngle"].GetDouble();

		assert((*itr)["param1"].HasMember("w"));
		assert((*itr)["param1"]["w"].IsDouble());
		light->param1.w = (float)(*itr)["param1"]["w"].GetDouble();

		//param2
		assert(itr->HasMember("param2"));
		assert((*itr)["param2"].IsObject());

		assert((*itr)["param2"].HasMember("isOn"));
		assert((*itr)["param2"]["isOn"].IsDouble());
		light->param2.x = (float)(*itr)["param2"]["isOn"].GetDouble();

		assert((*itr)["param2"].HasMember("y"));
		assert((*itr)["param2"]["y"].IsDouble());
		light->param2.y = (float)(*itr)["param2"]["y"].GetDouble();

		assert((*itr)["param2"].HasMember("z"));
		assert((*itr)["param2"]["z"].IsDouble());
		light->param2.z = (float)(*itr)["param2"]["z"].GetDouble();

		assert((*itr)["param2"].HasMember("w"));
		assert((*itr)["param2"]["w"].IsDouble());
		light->param2.z = (float)(*itr)["param2"]["w"].GetDouble();

		pScene->addLightToScene(light);

		lightCount++;
	}

	return true;
}

bool cSerialization::serializeSceneSounds(std::string fileName) {

	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

	std::vector<cSoundManager::sSoundInfo*> sounds = cSceneManager::getInstance()->getActiveScene()->getSounds();

	writer.StartObject();

	writer.String(("sounds"));
	writer.StartArray();
	for (std::vector<cSoundManager::sSoundInfo*>::iterator sound = sounds.begin(); sound != sounds.end(); ++sound) {

		writer.StartObject();
		(*sound)->serializeSound(writer);
		writer.EndObject();
	}
	writer.EndArray();

	writer.EndObject();

	// Write the JSON object to a file
	std::ofstream file("./assets/maps/" + fileName);

	if (!file.is_open()) {
		return false;
	}
	file << buffer.GetString();

	return true;
}

bool cSerialization::deserializeSceneSounds(std::string fileName) {

	std::ifstream ifs("./assets/maps/" + fileName);

	if (!ifs.is_open()) {
		return false;
	}

	rapidjson::IStreamWrapper isw(ifs);
	rapidjson::Document document;
	document.ParseStream(isw);

	// Get active scene
	cScene* pScene = cSceneManager::getInstance()->getActiveScene();

	assert(document.HasMember("sounds"));
	assert(document["sounds"].IsArray());

	rapidjson::GenericArray < false, rapidjson::Value::ValueType> sounds = (document["sounds"].GetArray());

	for (rapidjson::Value::ConstValueIterator compItr = sounds.Begin(); compItr != sounds.End(); ++compItr) {

		cSoundManager::sSoundInfo* soundInfo = new cSoundManager::sSoundInfo();

		//acceleration
		assert(compItr->HasMember("fileName"));
		assert((*compItr)["fileName"].IsString());
		soundInfo->fileName = (*compItr)["fileName"].GetString();

		cSoundManager::getInstance()->load3dSound(soundInfo, FMOD_LOOP_NORMAL | FMOD_3D_LINEARROLLOFF);

		assert(compItr->HasMember("soundType"));
		assert((*compItr)["soundType"].IsInt());
		soundInfo->type = (*compItr)["soundType"].GetInt();

		assert(compItr->HasMember("channelGroup"));
		assert((*compItr)["channelGroup"].IsInt());
		soundInfo->channelGroupIndex = (*compItr)["channelGroup"].GetInt();

		assert(compItr->HasMember("isPaused"));
		assert((*compItr)["isPaused"].IsBool());
		soundInfo->isPaused = (*compItr)["isPaused"].GetBool();

		assert(compItr->HasMember("isCompressed"));
		assert((*compItr)["isCompressed"].IsBool());
		soundInfo->isCompressed = (*compItr)["isCompressed"].GetBool();

		assert(compItr->HasMember("frequency"));
		assert((*compItr)["frequency"].IsDouble());
		soundInfo->frequency = (float)(*compItr)["frequency"].GetDouble();

		assert(compItr->HasMember("volume"));
		assert((*compItr)["volume"].IsDouble());
		soundInfo->volume = (float)(*compItr)["volume"].GetDouble();

		assert(compItr->HasMember("balance"));
		assert((*compItr)["balance"].IsDouble());
		soundInfo->balance = (float)(*compItr)["balance"].GetDouble();

		assert(compItr->HasMember("pitch"));
		assert((*compItr)["pitch"].IsDouble());
		soundInfo->pitch = (float)(*compItr)["pitch"].GetDouble();

		assert(compItr->HasMember("soundLength"));
		assert((*compItr)["soundLength"].IsUint());
		soundInfo->soundLength = (*compItr)["soundLength"].GetUint();

		assert(compItr->HasMember("channelPosition"));
		assert((*compItr)["channelPosition"].IsUint());
		soundInfo->channelPosition = (*compItr)["channelPosition"].GetUint();

		assert(compItr->HasMember("minDistance"));
		assert((*compItr)["minDistance"].IsDouble());
		soundInfo->f3dMinDistance = (float)(*compItr)["minDistance"].GetDouble();

		assert(compItr->HasMember("maxDistance"));
		assert((*compItr)["maxDistance"].IsDouble());
		soundInfo->f3dMaxDistance = (float)(*compItr)["maxDistance"].GetDouble();

		// "play the sound" to get a channels and others stuff
		cSoundManager::getInstance()->playSound(soundInfo);

		cSoundManager::getInstance()->setPaused(soundInfo, soundInfo->isPaused);
		cSoundManager::getInstance()->setSoundFrequency(*soundInfo, soundInfo->frequency);
		cSoundManager::getInstance()->setSoundVolume(*soundInfo, soundInfo->volume);
		cSoundManager::getInstance()->setSoundPitch(*soundInfo, soundInfo->pitch);
		cSoundManager::getInstance()->setSoundCurrentPosition(*soundInfo, soundInfo->channelPosition);

		pScene->addSoundToScene(soundInfo);

	}

	return true;
}

bool cSerialization::serializeSceneEntities(std::string fileName) {

	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

	std::vector<cEntity*> entities = cSceneManager::getInstance()->getActiveScene()->getEntities();

	writer.StartObject();

	writer.String(("entities"));
	writer.StartArray();
	for (std::vector<cEntity*>::iterator entity = entities.begin(); entity != entities.end(); ++entity) {
		(*entity)->serializeComponent(writer);
	}
	writer.EndArray();

	writer.EndObject();

	// Write the JSON object to a file
	std::ofstream file("./assets/maps/" + fileName);

	if (!file.is_open()) {
		return false;
	}
	file << buffer.GetString();


	return true;
}

bool cSerialization::deserializeSceneEntities(std::string fileName) {

	std::ifstream ifs("./assets/maps/" + fileName);

	if (!ifs.is_open()) {
		return false;
	}

	rapidjson::IStreamWrapper isw(ifs);
	rapidjson::Document document;
	document.ParseStream(isw);

	// Get active scene
	cScene* pScene = cSceneManager::getInstance()->getActiveScene();

	//entities
	assert(document.HasMember("entities"));
	assert(document["entities"].IsArray());
	rapidjson::GenericArray < false, rapidjson::Value::ValueType> entities = (document["entities"].GetArray()); //this is super templated library (the type is crazy)
	for (rapidjson::Value::ConstValueIterator itr = entities.Begin(); itr != entities.End(); ++itr) { // Ok

		assert((*itr).HasMember("entityType"));
		assert((*itr)["entityType"].IsInt());

		// create an instance of the entity
		cEntity* entity = cEntityBuilder::getInstance()->createEntity((*itr)["entityType"].GetInt());

		assert(itr->HasMember("id"));
		assert((*itr)["id"].IsInt());
		entity->UNIQUE_ID = (*itr)["id"].GetInt();

		// update the components to match the file
		assert((*itr).HasMember("components"));
		assert((*itr)["components"].IsArray());
		auto components = ((*itr)["components"].GetArray()); //this is super templated library (the type is crazy)
		for (rapidjson::Value::ConstValueIterator compItr = components.Begin(); compItr != components.End(); ++compItr) {

			assert((*compItr).HasMember("componentType"));
			assert((*compItr)["componentType"].IsString());

			std::string type = (*compItr)["componentType"].GetString();

			if (type == "Transform") {

				mCreateTransformComponent(entity, compItr);

				pScene->addEntityToScene(entity);
			}
			else if (type == "Euler") {

				mCreateEulerComponent(entity, compItr);
			}
			else if (type == "Render") {

				mCreateRenderComponent(entity, compItr);
			}
			else if (type == "Rigid Body") {

				mCreateRigidBodyComponent(entity, compItr);
			}
			else if (type == "Sound") {

				mCreateSoundComponent(entity, compItr);
			}
			else if (type == "Particle") {

				mCreateParticleComponent(entity, compItr);

			}
			else if (type == "Cloth") {

				mCreateClothComponent(entity, compItr);
			}
			else {

				std::cout << "Incompatible type in file" << std::endl;
			}
		}
	}

	return true;
}


bool cSerialization::deserializeConfigSettings(std::string fileName, sConfigSettings& settingsOut) {

	std::ifstream ifs("./assets/maps/" + fileName);

	if (!ifs.is_open()) {
		return false;
	}

	rapidjson::IStreamWrapper isw(ifs);
	rapidjson::Document document;
	document.ParseStream(isw);

	assert(document.HasMember("width"));
	assert(document["width"].IsInt());
	settingsOut.width = document["width"].GetInt();

	assert(document.HasMember("height"));
	assert(document["height"].IsInt());
	settingsOut.height = document["height"].GetInt();

	assert(document.HasMember("title"));
	assert(document["title"].IsString());
	settingsOut.title = document["title"].GetString();

	assert(document.HasMember("physicsDll"));
	assert(document["physicsDll"].IsString());
	settingsOut.physicsDll = document["physicsDll"].GetString();

	return true;
}

// Helper functions for deserialize entity
void cSerialization::mCreateTransformComponent(cEntity* entity, rapidjson::Value::ConstValueIterator& compItr) {

	cTransformComponent* transformComp = entity->getComponent<cTransformComponent>();
	glm::vec3 pos(0.0f);
	//position
	assert(compItr->HasMember("position"));
	assert((*compItr)["position"].IsObject());

	assert((*compItr)["position"].HasMember("x"));
	assert((*compItr)["position"]["x"].IsDouble());
	pos.x = (float)(*compItr)["position"]["x"].GetDouble();

	assert((*compItr)["position"].HasMember("y"));
	assert((*compItr)["position"]["y"].IsDouble());
	pos.y = (float)(*compItr)["position"]["y"].GetDouble();

	assert((*compItr)["position"].HasMember("z"));
	assert((*compItr)["position"]["z"].IsDouble());
	pos.z = (float)(*compItr)["position"]["z"].GetDouble();

	transformComp->setPosition(pos);

	//rotation
	assert(compItr->HasMember("orientation"));
	assert((*compItr)["orientation"].IsObject());

	glm::vec3 eulerOrientation;

	assert((*compItr)["orientation"].HasMember("x"));
	assert((*compItr)["orientation"]["x"].IsDouble());
	eulerOrientation.x = (float)(*compItr)["orientation"]["x"].GetDouble();

	assert((*compItr)["orientation"].HasMember("y"));
	assert((*compItr)["orientation"]["y"].IsDouble());
	eulerOrientation.y = (float)(*compItr)["orientation"]["y"].GetDouble();

	assert((*compItr)["orientation"].HasMember("z"));
	assert((*compItr)["orientation"]["z"].IsDouble());
	eulerOrientation.z = (float)(*compItr)["orientation"]["z"].GetDouble();

	transformComp->setOrientationEulerAngles(eulerOrientation, true);

	//scale
	assert(compItr->HasMember("scale"));
	assert((*compItr)["scale"].IsObject());

	assert((*compItr)["scale"].HasMember("x"));
	assert((*compItr)["scale"]["x"].IsDouble());
	transformComp->scale.x = (float)(*compItr)["scale"]["x"].GetDouble();

	assert((*compItr)["scale"].HasMember("y"));
	assert((*compItr)["scale"]["y"].IsDouble());
	transformComp->scale.y = (float)(*compItr)["scale"]["y"].GetDouble();

	assert((*compItr)["scale"].HasMember("z"));
	assert((*compItr)["scale"]["z"].IsDouble());
	transformComp->scale.z = (float)(*compItr)["scale"]["z"].GetDouble();

	return;
}

void cSerialization::mCreateEulerComponent(cEntity* entity, rapidjson::Value::ConstValueIterator& compItr) {

	cEulerMovementComponent* eulerMoveComp = entity->getComponent<cEulerMovementComponent>();

	//acceleration
	assert(compItr->HasMember("acceleration"));
	assert((*compItr)["acceleration"].IsObject());

	assert((*compItr)["acceleration"].HasMember("x"));
	assert((*compItr)["acceleration"]["x"].IsDouble());
	eulerMoveComp->acceleration.x = (float)(*compItr)["acceleration"]["x"].GetDouble();

	assert((*compItr)["acceleration"].HasMember("y"));
	assert((*compItr)["acceleration"]["y"].IsDouble());
	eulerMoveComp->acceleration.y = (float)(*compItr)["acceleration"]["y"].GetDouble();

	assert((*compItr)["acceleration"].HasMember("z"));
	assert((*compItr)["acceleration"]["z"].IsDouble());
	eulerMoveComp->acceleration.z = (float)(*compItr)["acceleration"]["z"].GetDouble();

	//velocity
	assert(compItr->HasMember("velocity"));
	assert((*compItr)["velocity"].IsObject());

	assert((*compItr)["velocity"].HasMember("x"));
	assert((*compItr)["velocity"]["x"].IsDouble());
	eulerMoveComp->velocity.x = (float)(*compItr)["velocity"]["x"].GetDouble();

	assert((*compItr)["velocity"].HasMember("y"));
	assert((*compItr)["velocity"]["y"].IsDouble());
	eulerMoveComp->velocity.y = (float)(*compItr)["velocity"]["y"].GetDouble();

	assert((*compItr)["velocity"].HasMember("z"));
	assert((*compItr)["velocity"]["z"].IsDouble());
	eulerMoveComp->velocity.z = (float)(*compItr)["velocity"]["z"].GetDouble();


	return;
}

void cSerialization::mCreateRenderComponent(cEntity* entity, rapidjson::Value::ConstValueIterator& compItr) {

	cRenderMeshComponent* renderComp = entity->getComponent<cRenderMeshComponent>();

	//acceleration
	assert(compItr->HasMember("meshName"));
	assert((*compItr)["meshName"].IsString());
	renderComp->meshName = (*compItr)["meshName"].GetString();

	assert(compItr->HasMember("friendlyName"));
	assert((*compItr)["friendlyName"].IsString());
	renderComp->friendlyName = (*compItr)["friendlyName"].GetString();

	assert(compItr->HasMember("isWireframe"));
	assert((*compItr)["isWireframe"].IsBool());
	renderComp->bIsWireFrame = (*compItr)["isWireframe"].GetBool();

	assert(compItr->HasMember("isVisible"));
	assert((*compItr)["isVisible"].IsBool());
	renderComp->bIsVisible = (*compItr)["isVisible"].GetBool();

	assert(compItr->HasMember("useVertexColor"));
	assert((*compItr)["useVertexColor"].IsBool());
	renderComp->bUseVertexColour = (*compItr)["useVertexColor"].GetBool();

	assert(compItr->HasMember("dontLight"));
	assert((*compItr)["dontLight"].IsBool());
	renderComp->bDontLight = (*compItr)["dontLight"].GetBool();

	assert(compItr->HasMember("color"));
	assert((*compItr)["color"].IsObject());

	assert((*compItr)["color"].HasMember("r"));
	assert((*compItr)["color"]["r"].IsDouble());
	renderComp->materialDiffuse.r = (float)(*compItr)["color"]["r"].GetDouble();

	assert((*compItr)["color"].HasMember("g"));
	assert((*compItr)["color"]["g"].IsDouble());
	renderComp->materialDiffuse.g = (float)(*compItr)["color"]["g"].GetDouble();

	assert((*compItr)["color"].HasMember("b"));
	assert((*compItr)["color"]["b"].IsDouble());
	renderComp->materialDiffuse.b = (float)(*compItr)["color"]["b"].GetDouble();

	assert((*compItr)["color"].HasMember("a"));
	assert((*compItr)["color"]["a"].IsDouble());
	renderComp->materialDiffuse.a = (float)(*compItr)["color"]["a"].GetDouble();

	//textures
	assert(compItr->HasMember("textures"));
	assert((*compItr)["textures"].IsArray());
	rapidjson::GenericArray < true, rapidjson::Value::ValueType> textures = ((*compItr)["textures"].GetArray());

	for (rapidjson::Value::ConstValueIterator texture = textures.Begin(); texture != textures.End(); ++texture) {
		sTextureInfo textureInfo;

		assert(texture->HasMember("name"));
		assert((*texture)["name"].IsString());
		textureInfo.name = (*texture)["name"].GetString();

		assert(texture->HasMember("strength"));
		assert((*texture)["strength"].IsDouble());
		textureInfo.strength = (float)(*texture)["strength"].GetDouble();

		renderComp->textures.push_back(textureInfo);
	}

	assert(compItr->HasMember("specular"));
	assert((*compItr)["specular"].IsObject());

	assert((*compItr)["specular"].HasMember("r"));
	assert((*compItr)["specular"]["r"].IsDouble());
	renderComp->materialSpecular.r = (float)(*compItr)["specular"]["r"].GetDouble();

	assert((*compItr)["specular"].HasMember("g"));
	assert((*compItr)["specular"]["g"].IsDouble());
	renderComp->materialSpecular.g = (float)(*compItr)["specular"]["g"].GetDouble();

	assert((*compItr)["specular"].HasMember("b"));
	assert((*compItr)["specular"]["b"].IsDouble());
	renderComp->materialSpecular.b = (float)(*compItr)["specular"]["b"].GetDouble();

	assert((*compItr)["specular"].HasMember("shinyness"));
	assert((*compItr)["specular"]["shinyness"].IsDouble());
	renderComp->materialSpecular.a = (float)(*compItr)["specular"]["shinyness"].GetDouble();


	return;
}

void cSerialization::mCreateRigidBodyComponent(cEntity* entity, rapidjson::Value::ConstValueIterator& compItr) {

	cRigidBodyComponent* physicsComp = entity->getComponent<cRigidBodyComponent>();
	cEngine* pEngine = cEngine::getInstance();

	assert(compItr->HasMember("shape"));
	assert((*compItr)["shape"].IsObject());

	assert((*compItr)["shape"].HasMember("type"));
	assert((*compItr)["shape"]["type"].IsString());
	std::string type = (*compItr)["shape"]["type"].GetString();

	nPhysics::iShape* pShape = nullptr;

	if (type == "sphere") {

		assert((*compItr)["shape"].HasMember("radius"));
		assert((*compItr)["shape"]["radius"].IsDouble());
		float radius = (*compItr)["shape"]["radius"].GetDouble();

		pShape = pEngine->getPhysicsFactory()->createSphere(radius);
	}
	else if (type == "plane") {

		assert((*compItr)["shape"].HasMember("normal"));
		assert((*compItr)["shape"]["normal"].IsObject());

		glm::vec3 normal;

		assert((*compItr)["shape"]["normal"].HasMember("x"));
		assert((*compItr)["shape"]["normal"]["x"].IsDouble());
		normal.x = (*compItr)["shape"]["normal"]["x"].GetDouble();

		assert((*compItr)["shape"]["normal"].HasMember("y"));
		assert((*compItr)["shape"]["normal"]["y"].IsDouble());
		normal.y = (*compItr)["shape"]["normal"]["y"].GetDouble();

		assert((*compItr)["shape"]["normal"].HasMember("z"));
		assert((*compItr)["shape"]["normal"]["z"].IsDouble());
		normal.z = (*compItr)["shape"]["normal"]["z"].GetDouble();

		assert((*compItr)["shape"].HasMember("constant"));
		assert((*compItr)["shape"]["constant"].IsDouble());
		float constant = (*compItr)["shape"]["constant"].GetDouble();

		pShape = pEngine->getPhysicsFactory()->createPlane(normal, constant);
	}

	nPhysics::sRigidBodyDef def;

	//position
	assert(compItr->HasMember("position"));
	assert((*compItr)["position"].IsObject());

	assert((*compItr)["position"].HasMember("x"));
	assert((*compItr)["position"]["x"].IsDouble());
	def.position.x = (float)(*compItr)["position"]["x"].GetDouble();

	assert((*compItr)["position"].HasMember("y"));
	assert((*compItr)["position"]["y"].IsDouble());
	def.position.y = (float)(*compItr)["position"]["y"].GetDouble();

	assert((*compItr)["position"].HasMember("z"));
	assert((*compItr)["position"]["z"].IsDouble());
	def.position.z = (float)(*compItr)["position"]["z"].GetDouble();

	// orientation
	assert(compItr->HasMember("orientation"));
	assert((*compItr)["orientation"].IsObject());

	assert((*compItr)["orientation"].HasMember("x"));
	assert((*compItr)["orientation"]["x"].IsDouble());
	def.orientation.x = (float)(*compItr)["orientation"]["x"].GetDouble();

	assert((*compItr)["orientation"].HasMember("y"));
	assert((*compItr)["orientation"]["y"].IsDouble());
	def.orientation.y = (float)(*compItr)["orientation"]["y"].GetDouble();

	assert((*compItr)["orientation"].HasMember("z"));
	assert((*compItr)["orientation"]["z"].IsDouble());
	def.orientation.z = (float)(*compItr)["orientation"]["z"].GetDouble();

	//velocity
	assert(compItr->HasMember("velocity"));
	assert((*compItr)["velocity"].IsObject());

	assert((*compItr)["velocity"].HasMember("x"));
	assert((*compItr)["velocity"]["x"].IsDouble());
	def.velocity.x = (float)(*compItr)["velocity"]["x"].GetDouble();

	assert((*compItr)["velocity"].HasMember("y"));
	assert((*compItr)["velocity"]["y"].IsDouble());
	def.velocity.y = (float)(*compItr)["velocity"]["y"].GetDouble();

	assert((*compItr)["velocity"].HasMember("z"));
	assert((*compItr)["velocity"]["z"].IsDouble());
	def.velocity.z = (float)(*compItr)["velocity"]["z"].GetDouble();

	//acceleration
	assert(compItr->HasMember("acceleration"));
	assert((*compItr)["acceleration"].IsObject());

	assert((*compItr)["acceleration"].HasMember("x"));
	assert((*compItr)["acceleration"]["x"].IsDouble());
	def.acceleration.x = (float)(*compItr)["acceleration"]["x"].GetDouble();

	assert((*compItr)["acceleration"].HasMember("y"));
	assert((*compItr)["acceleration"]["y"].IsDouble());
	def.acceleration.y = (float)(*compItr)["acceleration"]["y"].GetDouble();

	assert((*compItr)["acceleration"].HasMember("z"));
	assert((*compItr)["acceleration"]["z"].IsDouble());
	def.acceleration.z = (float)(*compItr)["acceleration"]["z"].GetDouble();

	// mass
	assert(compItr->HasMember("mass"));
	assert((*compItr)["mass"].IsDouble());
	def.mass = (float)(*compItr)["mass"].GetDouble();

	nPhysics::iRigidBody* pBody = nullptr;
	if (pShape != nullptr) {
		pBody = pEngine->getPhysicsFactory()->createRigidBody(def, pShape);
	}

	physicsComp->setRigidBody(pBody);
	pEngine->getPhysicsWorld()->addCollisionBody(pBody);

	return;
}

void cSerialization::mCreateSoundComponent(cEntity* entity, rapidjson::Value::ConstValueIterator& compItr) {

	c3dSoundComponent* soundComp = entity->getComponent<c3dSoundComponent>();

	//acceleration
	assert(compItr->HasMember("fileName"));
	assert((*compItr)["fileName"].IsString());
	soundComp->soundInfo->fileName = (*compItr)["fileName"].GetString();

	cSoundManager::getInstance()->load3dSound(soundComp->soundInfo, FMOD_LOOP_NORMAL | FMOD_3D_LINEARROLLOFF);

	assert(compItr->HasMember("soundType"));
	assert((*compItr)["soundType"].IsInt());
	soundComp->soundInfo->type = (*compItr)["soundType"].GetInt();

	assert(compItr->HasMember("channelGroup"));
	assert((*compItr)["channelGroup"].IsInt());
	soundComp->soundInfo->channelGroupIndex = (*compItr)["channelGroup"].GetInt();

	assert(compItr->HasMember("isPaused"));
	assert((*compItr)["isPaused"].IsBool());
	soundComp->soundInfo->isPaused = (*compItr)["isPaused"].GetBool();

	assert(compItr->HasMember("isCompressed"));
	assert((*compItr)["isCompressed"].IsBool());
	soundComp->soundInfo->isCompressed = (*compItr)["isCompressed"].GetBool();

	assert(compItr->HasMember("frequency"));
	assert((*compItr)["frequency"].IsDouble());
	soundComp->soundInfo->frequency = (float)(*compItr)["frequency"].GetDouble();

	assert(compItr->HasMember("volume"));
	assert((*compItr)["volume"].IsDouble());
	soundComp->soundInfo->volume = (float)(*compItr)["volume"].GetDouble();

	assert(compItr->HasMember("balance"));
	assert((*compItr)["balance"].IsDouble());
	soundComp->soundInfo->balance = (float)(*compItr)["balance"].GetDouble();

	assert(compItr->HasMember("pitch"));
	assert((*compItr)["pitch"].IsDouble());
	soundComp->soundInfo->pitch = (float)(*compItr)["pitch"].GetDouble();

	assert(compItr->HasMember("soundLength"));
	assert((*compItr)["soundLength"].IsUint());
	soundComp->soundInfo->soundLength = (*compItr)["soundLength"].GetUint();

	assert(compItr->HasMember("channelPosition"));
	assert((*compItr)["channelPosition"].IsUint());
	soundComp->soundInfo->channelPosition = (*compItr)["channelPosition"].GetUint();

	assert(compItr->HasMember("minDistance"));
	assert((*compItr)["minDistance"].IsDouble());
	soundComp->soundInfo->f3dMinDistance = (float)(*compItr)["minDistance"].GetDouble();

	assert(compItr->HasMember("maxDistance"));
	assert((*compItr)["maxDistance"].IsDouble());
	soundComp->soundInfo->f3dMaxDistance = (float)(*compItr)["maxDistance"].GetDouble();

	// "play the sound" to get a channels and others stuff
	cSoundManager::getInstance()->playSound(soundComp->soundInfo);

	//move the 3d sound
	cTransformComponent* transComp = entity->getComponent<cTransformComponent>();
	FMOD_VECTOR position;
	glm::vec3 pos(0.0f);
	position.x = pos.x;
	position.y = pos.y;
	position.z = pos.z;

	FMOD_VECTOR velocity = { 0.0f,0.0f,0.0f };

	cSoundManager::getInstance()->mresult = soundComp->soundInfo->channel->set3DAttributes(&position, &velocity);
	cSoundManager::getInstance()->checkError();

	cSoundManager::getInstance()->mresult = soundComp->soundInfo->channel->set3DMinMaxDistance(soundComp->soundInfo->f3dMinDistance, soundComp->soundInfo->f3dMaxDistance);
	cSoundManager::getInstance()->checkError();

	cSoundManager::getInstance()->setPaused(soundComp->soundInfo, soundComp->soundInfo->isPaused);
	cSoundManager::getInstance()->setSoundFrequency(*soundComp->soundInfo, soundComp->soundInfo->frequency);
	cSoundManager::getInstance()->setSoundVolume(*soundComp->soundInfo, soundComp->soundInfo->volume);
	cSoundManager::getInstance()->setSoundPitch(*soundComp->soundInfo, soundComp->soundInfo->pitch);
	cSoundManager::getInstance()->setSoundCurrentPosition(*soundComp->soundInfo, soundComp->soundInfo->channelPosition);

	cSoundManager::getInstance()->update();

	return;
}

void cSerialization::mCreateParticleComponent(cEntity* entity, rapidjson::Value::ConstValueIterator& compItr) {

	cParticleComponent* particleComp = entity->getComponent<cParticleComponent>();
	cParticleComponent::sParticleCreationParams particleParams;

	assert(compItr->HasMember("minLifeTime"));
	assert((*compItr)["minLifeTime"].IsDouble());
	particleParams.minLifeTime = (float)(*compItr)["minLifeTime"].GetDouble();

	assert(compItr->HasMember("maxLifeTime"));
	assert((*compItr)["maxLifeTime"].IsDouble());
	particleParams.maxLifeTime = (float)(*compItr)["maxLifeTime"].GetDouble();

	assert(compItr->HasMember("totalParticles"));
	assert((*compItr)["totalParticles"].IsUint());
	particleParams.totalNumberOfParticles = (*compItr)["totalParticles"].GetUint();

	//position
	assert(compItr->HasMember("position"));
	assert((*compItr)["position"].IsObject());

	assert((*compItr)["position"].HasMember("x"));
	assert((*compItr)["position"]["x"].IsDouble());
	particleParams.position.x = (float)(*compItr)["position"]["x"].GetDouble();

	assert((*compItr)["position"].HasMember("y"));
	assert((*compItr)["position"]["y"].IsDouble());
	particleParams.position.y = (float)(*compItr)["position"]["y"].GetDouble();

	assert((*compItr)["position"].HasMember("z"));
	assert((*compItr)["position"]["z"].IsDouble());
	particleParams.position.z = (float)(*compItr)["position"]["z"].GetDouble();

	// min intital velocity
	assert(compItr->HasMember("minInitialVelocity"));
	assert((*compItr)["minInitialVelocity"].IsObject());

	assert((*compItr)["minInitialVelocity"].HasMember("x"));
	assert((*compItr)["minInitialVelocity"]["x"].IsDouble());
	particleParams.minInitialVelocity.x = (float)(*compItr)["minInitialVelocity"]["x"].GetDouble();

	assert((*compItr)["minInitialVelocity"].HasMember("y"));
	assert((*compItr)["minInitialVelocity"]["y"].IsDouble());
	particleParams.minInitialVelocity.y = (float)(*compItr)["minInitialVelocity"]["y"].GetDouble();

	assert((*compItr)["minInitialVelocity"].HasMember("z"));
	assert((*compItr)["minInitialVelocity"]["z"].IsDouble());
	particleParams.minInitialVelocity.z = (float)(*compItr)["minInitialVelocity"]["z"].GetDouble();

	// max initial velocity
	assert(compItr->HasMember("maxInitialVelocity"));
	assert((*compItr)["maxInitialVelocity"].IsObject());

	assert((*compItr)["maxInitialVelocity"].HasMember("x"));
	assert((*compItr)["maxInitialVelocity"]["x"].IsDouble());
	particleParams.maxInitialVelocity.x = (float)(*compItr)["maxInitialVelocity"]["x"].GetDouble();

	assert((*compItr)["maxInitialVelocity"].HasMember("y"));
	assert((*compItr)["maxInitialVelocity"]["y"].IsDouble());
	particleParams.maxInitialVelocity.y = (float)(*compItr)["maxInitialVelocity"]["y"].GetDouble();

	assert((*compItr)["maxInitialVelocity"].HasMember("z"));
	assert((*compItr)["maxInitialVelocity"]["z"].IsDouble());
	particleParams.maxInitialVelocity.z = (float)(*compItr)["maxInitialVelocity"]["z"].GetDouble();

	// acceleration
	assert(compItr->HasMember("acceleration"));
	assert((*compItr)["acceleration"].IsObject());

	assert((*compItr)["acceleration"].HasMember("x"));
	assert((*compItr)["acceleration"]["x"].IsDouble());
	particleParams.acceleration.x = (float)(*compItr)["acceleration"]["x"].GetDouble();

	assert((*compItr)["acceleration"].HasMember("y"));
	assert((*compItr)["acceleration"]["y"].IsDouble());
	particleParams.acceleration.y = (float)(*compItr)["acceleration"]["y"].GetDouble();

	assert((*compItr)["acceleration"].HasMember("z"));
	assert((*compItr)["acceleration"]["z"].IsDouble());
	particleParams.acceleration.z = (float)(*compItr)["acceleration"]["z"].GetDouble();

	// min initial orientation
	assert(compItr->HasMember("minInitialOrientation"));
	assert((*compItr)["minInitialOrientation"].IsObject());

	assert((*compItr)["minInitialOrientation"].HasMember("x"));
	assert((*compItr)["minInitialOrientation"]["x"].IsDouble());
	particleParams.minInitialOrientationEuler.x = (float)(*compItr)["minInitialOrientation"]["x"].GetDouble();

	assert((*compItr)["minInitialOrientation"].HasMember("y"));
	assert((*compItr)["minInitialOrientation"]["y"].IsDouble());
	particleParams.minInitialOrientationEuler.y = (float)(*compItr)["minInitialOrientation"]["y"].GetDouble();

	assert((*compItr)["minInitialOrientation"].HasMember("z"));
	assert((*compItr)["minInitialOrientation"]["z"].IsDouble());
	particleParams.minInitialOrientationEuler.z = (float)(*compItr)["minInitialOrientation"]["z"].GetDouble();

	// max initial orientation
	assert(compItr->HasMember("maxInitialOrientation"));
	assert((*compItr)["maxInitialOrientation"].IsObject());

	assert((*compItr)["maxInitialOrientation"].HasMember("x"));
	assert((*compItr)["maxInitialOrientation"]["x"].IsDouble());
	particleParams.maxInitialOrientationEuler.x = (float)(*compItr)["maxInitialOrientation"]["x"].GetDouble();

	assert((*compItr)["maxInitialOrientation"].HasMember("y"));
	assert((*compItr)["maxInitialOrientation"]["y"].IsDouble());
	particleParams.maxInitialOrientationEuler.y = (float)(*compItr)["maxInitialOrientation"]["y"].GetDouble();

	assert((*compItr)["maxInitialOrientation"].HasMember("z"));
	assert((*compItr)["maxInitialOrientation"]["z"].IsDouble());
	particleParams.maxInitialOrientationEuler.z = (float)(*compItr)["maxInitialOrientation"]["z"].GetDouble();

	// min orientation change
	assert(compItr->HasMember("minOrientationChange"));
	assert((*compItr)["minOrientationChange"].IsObject());

	assert((*compItr)["minOrientationChange"].HasMember("x"));
	assert((*compItr)["minOrientationChange"]["x"].IsDouble());
	particleParams.minOrientationChangeEuler.x = (float)(*compItr)["minOrientationChange"]["x"].GetDouble();

	assert((*compItr)["minOrientationChange"].HasMember("y"));
	assert((*compItr)["minOrientationChange"]["y"].IsDouble());
	particleParams.minOrientationChangeEuler.y = (float)(*compItr)["minOrientationChange"]["y"].GetDouble();

	assert((*compItr)["minOrientationChange"].HasMember("z"));
	assert((*compItr)["minOrientationChange"]["z"].IsDouble());
	particleParams.minOrientationChangeEuler.z = (float)(*compItr)["minOrientationChange"]["z"].GetDouble();

	// max orientation change
	assert(compItr->HasMember("maxOrientationChange"));
	assert((*compItr)["maxOrientationChange"].IsObject());

	assert((*compItr)["maxOrientationChange"].HasMember("x"));
	assert((*compItr)["maxOrientationChange"]["x"].IsDouble());
	particleParams.maxOrientationChangeEuler.x = (float)(*compItr)["maxOrientationChange"]["x"].GetDouble();

	assert((*compItr)["maxOrientationChange"].HasMember("y"));
	assert((*compItr)["maxOrientationChange"]["y"].IsDouble());
	particleParams.maxOrientationChangeEuler.y = (float)(*compItr)["maxOrientationChange"]["y"].GetDouble();

	assert((*compItr)["maxOrientationChange"].HasMember("z"));
	assert((*compItr)["maxOrientationChange"]["z"].IsDouble());
	particleParams.maxOrientationChangeEuler.z = (float)(*compItr)["maxOrientationChange"]["z"].GetDouble();

	assert(compItr->HasMember("minInitialScale"));
	assert((*compItr)["minInitialScale"].IsDouble());
	particleParams.minInitialScale = (float)(*compItr)["minInitialScale"].GetDouble();

	assert(compItr->HasMember("maxInitialScale"));
	assert((*compItr)["maxInitialScale"].IsDouble());
	particleParams.maxInitialScale = (float)(*compItr)["maxInitialScale"].GetDouble();

	assert(compItr->HasMember("minScaleChange"));
	assert((*compItr)["minScaleChange"].IsDouble());
	particleParams.minScaleChange = (float)(*compItr)["minScaleChange"].GetDouble();

	assert(compItr->HasMember("maxScaleChange"));
	assert((*compItr)["maxScaleChange"].IsDouble());
	particleParams.maxScaleChange = (float)(*compItr)["maxScaleChange"].GetDouble();

	assert(compItr->HasMember("deathFadeTime"));
	assert((*compItr)["deathFadeTime"].IsDouble());
	particleParams.deathTransparencyFadeTimeSeconds = (float)(*compItr)["deathFadeTime"].GetDouble();

	assert(compItr->HasMember("minNewParticles"));
	assert((*compItr)["minNewParticles"].IsUint());
	particleParams.minNumberNewParticles = (*compItr)["minNewParticles"].GetUint();

	assert(compItr->HasMember("maxNewParticles"));
	assert((*compItr)["maxNewParticles"].IsUint());
	particleParams.maxNumberNewParticles = (*compItr)["maxNewParticles"].GetUint();

	assert(compItr->HasMember("minTimeBetweenGen"));
	assert((*compItr)["minTimeBetweenGen"].IsDouble());
	particleParams.minTimeBetweenParticleGenSeconds = (float)(*compItr)["minTimeBetweenGen"].GetDouble();

	assert(compItr->HasMember("maxTimeBetweenGen"));
	assert((*compItr)["maxTimeBetweenGen"].IsDouble());
	particleParams.maxTimeBetweenParticleGenSeconds = (float)(*compItr)["maxTimeBetweenGen"].GetDouble();

	particleComp->init(particleParams);

	return;
}

void cSerialization::mCreateClothComponent(cEntity* entity, rapidjson::Value::ConstValueIterator& compItr) {

	cClothComponent* pClothComp = entity->getComponent<cClothComponent>();
	cEngine* pEngine = cEngine::getInstance();

	assert(compItr->HasMember("numNodesAB"));
	assert((*compItr)["numNodesAB"].IsInt());
	size_t numNodesAB = (*compItr)["numNodesAB"].GetInt();

	assert(compItr->HasMember("numNodesAC"));
	assert((*compItr)["numNodesAC"].IsInt());
	size_t numNodesAC = (*compItr)["numNodesAC"].GetInt();
	
	//Corner A
	assert(compItr->HasMember("CornerA"));
	assert((*compItr)["CornerA"].IsObject());

	glm::vec3 cornerA(0.0f);
	assert((*compItr)["CornerA"].HasMember("x"));
	assert((*compItr)["CornerA"]["x"].IsDouble());
	cornerA.x = (float)(*compItr)["CornerA"]["x"].GetDouble();

	assert((*compItr)["CornerA"].HasMember("y"));
	assert((*compItr)["CornerA"]["y"].IsDouble());
	cornerA.y = (float)(*compItr)["CornerA"]["y"].GetDouble();

	assert((*compItr)["CornerA"].HasMember("z"));
	assert((*compItr)["CornerA"]["z"].IsDouble());
	cornerA.z = (float)(*compItr)["CornerA"]["z"].GetDouble();

	// Corner B
	assert(compItr->HasMember("CornerB"));
	assert((*compItr)["CornerB"].IsObject());

	glm::vec3 cornerB(0.0f);
	assert((*compItr)["CornerB"].HasMember("x"));
	assert((*compItr)["CornerB"]["x"].IsDouble());
	cornerB.x = (float)(*compItr)["CornerB"]["x"].GetDouble();

	assert((*compItr)["CornerB"].HasMember("y"));
	assert((*compItr)["CornerB"]["y"].IsDouble());
	cornerB.y = (float)(*compItr)["CornerB"]["y"].GetDouble();

	assert((*compItr)["CornerB"].HasMember("z"));
	assert((*compItr)["CornerB"]["z"].IsDouble());
	cornerB.z = (float)(*compItr)["CornerB"]["z"].GetDouble();

	// Corner C
	assert(compItr->HasMember("CornerC"));
	assert((*compItr)["CornerC"].IsObject());

	glm::vec3 cornerC(0.0f);
	assert((*compItr)["CornerC"].HasMember("x"));
	assert((*compItr)["CornerC"]["x"].IsDouble());
	cornerC.x = (float)(*compItr)["CornerC"]["x"].GetDouble();

	assert((*compItr)["CornerC"].HasMember("y"));
	assert((*compItr)["CornerC"]["y"].IsDouble());
	cornerC.y = (float)(*compItr)["CornerC"]["y"].GetDouble();

	assert((*compItr)["CornerC"].HasMember("z"));
	assert((*compItr)["CornerC"]["z"].IsDouble());
	cornerC.z = (float)(*compItr)["CornerC"]["z"].GetDouble();

	// Spring Constant 
	assert(compItr->HasMember("springConstant"));
	assert((*compItr)["springConstant"].IsDouble());
	float springConstant = (float)(*compItr)["springConstant"].GetDouble();

	// Damping Factor
	assert(compItr->HasMember("dampingFactor"));
	assert((*compItr)["dampingFactor"].IsDouble());
	float dampingFactor = (float)(*compItr)["dampingFactor"].GetDouble();

	// Check that the damping factor is between 0 and 1.0f
	assert(dampingFactor >= 0.0f && dampingFactor <= 1.0f, "The damping factor needs to be between 0.0f and 1.0f");
	if (dampingFactor < 0.0f || dampingFactor > 1.0f) {

		std::cout << "The damping factor needs to be between 0.0f and 1.0f" << std::endl;
	}

	// Wind variables
	assert(compItr->HasMember("minWindForce"));
	assert((*compItr)["minWindForce"].IsDouble());
	float minWind = (float)(*compItr)["minWindForce"].GetDouble();

	assert(compItr->HasMember("maxWindForce"));
	assert((*compItr)["maxWindForce"].IsDouble());
	float maxWind = (float)(*compItr)["maxWindForce"].GetDouble();

	assert(compItr->HasMember("windDirection"));
	assert((*compItr)["windDirection"].IsObject());

	glm::vec3 windDirection(0.0f);
	assert((*compItr)["windDirection"].HasMember("x"));
	assert((*compItr)["windDirection"]["x"].IsDouble());
	windDirection.x = (float)(*compItr)["windDirection"]["x"].GetDouble();

	assert((*compItr)["windDirection"].HasMember("y"));
	assert((*compItr)["windDirection"]["y"].IsDouble());
	windDirection.y = (float)(*compItr)["windDirection"]["y"].GetDouble();

	assert((*compItr)["windDirection"].HasMember("z"));
	assert((*compItr)["windDirection"]["z"].IsDouble());
	windDirection.z = (float)(*compItr)["windDirection"]["z"].GetDouble();
	
	nPhysics::sSoftBodyDef def;
	def.springConstant = springConstant;
	def.dampingFactor = dampingFactor;
	def.minWindForce = minWind;
	def.maxWindForce = maxWind;
	def.windDirection = windDirection;

	// Create the nodes
	glm::vec3 position;
	glm::vec3 sepAtoB((cornerB - cornerA) / float(numNodesAB - 1)); // TODO: Divide by zero error
	glm::vec3 sepAtoC((cornerC - cornerA) / float(numNodesAC - 1));
	
	for (size_t idxB = 0; idxB < numNodesAC; idxB++) {

		for (size_t idxA = 0; idxA < numNodesAB; idxA++) {

			glm::vec3 position = cornerA + sepAtoB * float(idxA) + sepAtoC * float(idxB);
			def.nodes.push_back(nPhysics::sSoftBodyNodeDef(position, 1.0f));
		}
	}

	// Set the top row of nodes as fixed
	for (size_t idxA = 0; idxA < numNodesAB; idxA++) {

		def.nodes[idxA].mass = 0.0f;
	}

	// springs setup
	for (size_t idxB = 0; idxB < numNodesAC - 1; idxB++) { // Vertical springs
		// Subtracted 1 from numNodesAtoB to avoid running off the end of the vector

		for (size_t idxA = 0; idxA < numNodesAB - 1; idxA++) { // Horizontal springs

			size_t index = idxA + idxB * numNodesAB;
			def.springs.push_back(std::make_pair(index, index + 1));
			def.springs.push_back(std::make_pair(index, index + numNodesAB)); // Node right below the current node
		}
	}

	// Last horizontal row
	size_t numNodes = numNodesAB * numNodesAC;
	for (size_t idxA = 0; idxA < numNodesAB - 1; idxA++) {

		def.springs.push_back(std::make_pair(numNodes - idxA - 1, numNodes - idxA - 2));
	}

	// Last column vertically
	for (size_t idxB = 1; idxB < numNodesAC; idxB++) {

		size_t index = (numNodesAB * idxB) - 1;
		def.springs.push_back(std::make_pair(index, index + numNodesAB));
	}

	nPhysics::iSoftBody* pBody = nullptr;
	pBody = pEngine->getPhysicsFactory()->createSoftBody(def);
	

	pClothComp->setSoftBody(pBody);
	pEngine->getPhysicsWorld()->addCollisionBody(pBody);
	
	return;
}
/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#ifndef _cSerialization_HG_
#define _cSerialization_HG_

#include <string>
#include <rapidjson/document.h>
#include "cEntity.h"
#include "sConfigSettings.h"

class cSerialization {
public:
	static bool serializeSceneCamera(std::string fileName);
	static bool deserializeSceneCamera(std::string fileName);

	static bool serializeSceneLights(std::string fileName);
	static bool deserializeSceneLights(std::string fileName);

	static bool serializeSceneSounds(std::string fileName);
	static bool deserializeSceneSounds(std::string fileName);

	static bool serializeSceneEntities(std::string fileName);
	static bool deserializeSceneEntities(std::string fileName);

	static bool deserializeConfigSettings(std::string fileName, sConfigSettings& settingsOut);

private:
	static void mCreateTransformComponent(cEntity* entity, rapidjson::Value::ConstValueIterator& compItr);
	static void mCreateEulerComponent(cEntity* entity, rapidjson::Value::ConstValueIterator& compItr);
	static void mCreateRenderComponent(cEntity* entity, rapidjson::Value::ConstValueIterator& compItr);
	static void mCreateRigidBodyComponent(cEntity* entity, rapidjson::Value::ConstValueIterator& compItr);
	static void mCreateSoundComponent(cEntity* entity, rapidjson::Value::ConstValueIterator& compItr);
	static void mCreateParticleComponent(cEntity* entity, rapidjson::Value::ConstValueIterator& compItr);
	static void mCreateClothComponent(cEntity* entity, rapidjson::Value::ConstValueIterator& compItr);
};

#endif

#ifndef _c3dSoundComponent_HG_
#define _c3dSoundComponent_HG_

#include <string>

#include "cComponent.h"
#include "cSoundManager.h"

class c3dSoundComponent : public cComponent {
public:
	cSoundManager::sSoundInfo* soundInfo;

	c3dSoundComponent(cSoundManager::sSoundInfo* info) : cComponent(), soundInfo(info) {
		
		this->mType = "Sound";
	}

	void serializeComponent(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) {
		writer.StartObject();

		writer.String("componentType");
		writer.String(this->mType.c_str());
		
		this->soundInfo->serializeSound(writer);

		writer.EndObject();
	}

	~c3dSoundComponent() {
		delete this->soundInfo;
	}

private:
	std::string mType;
};

#endif
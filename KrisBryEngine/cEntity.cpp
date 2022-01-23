#include "cEntity.h"
#include "cEulerMovementComponent.h"
#include "cRenderMeshComponent.h"
#include "cTransformComponent.h"
#include "c3dSoundComponent.h"
#include "cParticleComponent.h"

cEntity::cEntity(int type) : UNIQUE_ID(++nextUniqueId), entityType(type) {}

cEntity::~cEntity() {

	for (unsigned int cmpIndex = 0; cmpIndex < this->components.size(); cmpIndex++) {
		
		this->removeComponent(this->components[cmpIndex]);
	}

	return;
}

void cEntity::removeComponent(cComponent* comp) {

	for (unsigned int i = 0; i < this->components.size(); i++) {
		
		if (this->components[i] == comp) {
			
			delete this->components[i];

			if (this->components.size() > 0) {
				
				this->components[i] = this->components[components.size()];
			}
			
			this->components.pop_back();
			
			return;
		}
	}

	return;
}

void cEntity::serializeComponent(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) {

	writer.StartObject();
	writer.String("id");
	writer.Int(this->UNIQUE_ID);

	writer.String("entityType");
	writer.Int(this->entityType);

	// Call serialize for each component
	writer.String(("components"));
	writer.StartArray();

	if (this->hasComponent<cTransformComponent>()) {

		this->getComponent<cTransformComponent>()->serializeComponent(writer);
	}

	if (this->hasComponent<cEulerMovementComponent>()) {

		this->getComponent<cEulerMovementComponent>()->serializeComponent(writer);
	}

	if (this->hasComponent<cRenderMeshComponent>()) {

		this->getComponent<cRenderMeshComponent>()->serializeComponent(writer);
	}

	if (this->hasComponent<c3dSoundComponent>()) {
	
		this->getComponent<c3dSoundComponent>()->serializeComponent(writer);
	}

	if (this->hasComponent<cParticleComponent>()) {
		
		this->getComponent<cParticleComponent>()->serializeComponent(writer);
	}

	writer.EndArray();
	writer.EndObject();

}

int cEntity::nextUniqueId = 0;

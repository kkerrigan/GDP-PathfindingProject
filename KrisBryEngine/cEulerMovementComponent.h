#ifndef _cEulerMovementComponent_HG_
#define _cEulerMovementComponent_HG_

#include <gameMath.h>
#include <string>
#include <rapidjson/prettywriter.h>

#include "cComponent.h"

class cEulerMovementComponent : public cComponent {
public:

	glm::vec3 acceleration;
	glm::vec3 velocity;

	cEulerMovementComponent() : cComponent(), acceleration(glm::vec3(0.0f)), velocity(glm::vec3(0.0f)) {
		
		this->mType = "Euler";
	}
	~cEulerMovementComponent() {}

	void serializeComponent(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) {
		writer.StartObject();
		
		writer.String("componentType");
		writer.String(this->mType.c_str());

		writer.String("acceleration");
		writer.StartObject();
			writer.String("x");
			writer.Double(this->acceleration.x);
			writer.String("y");
			writer.Double(this->acceleration.y);
			writer.String("z");
			writer.Double(this->acceleration.z);
		writer.EndObject();

		writer.String("velocity");
		writer.StartObject();
			writer.String("x");
			writer.Double(this->acceleration.x);
			writer.String("y");
			writer.Double(this->acceleration.y);
			writer.String("z");
			writer.Double(this->acceleration.z);
		writer.EndObject();
		
		writer.EndObject();
	}

private:
	std::string mType;
};

#endif

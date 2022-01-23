#ifndef _cTransformComponent_HG_
#define _cTransformComponent_HG_

#include <gameMath.h>
#include <string>

#include "cComponent.h"

#include <rapidjson/prettywriter.h>

class cTransformComponent : public cComponent {
public:
	glm::quat orientation;
	glm::vec3 scale;

	cTransformComponent();
	virtual ~cTransformComponent();

	void serializeComponent(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
	glm::quat getQOrientation(void);
	void setQOrientation(glm::quat newOrientation);
	void setOrientationEulerAngles(glm::vec3 newAnglesEuler, bool bIsDegrees = false);
	void setOrientationEulerAngles(float x, float y, float z, bool bIsDegrees = false);
	void adjOrientationEulerAngles(glm::vec3 adjAngleEuler, bool bIsDegrees = false);
	void adjOrientationEulerAngles(float x, float y, float z, bool bIsDegrees = false);
	void adjMeshOrientationQ(glm::quat adjOrientQ);

	void setUniformScale(float scale);

	glm::vec3 getPosition();
	void setPosition(glm::vec3);

private:
	glm::vec3 mPosition;
	std::string mType;
};

#endif
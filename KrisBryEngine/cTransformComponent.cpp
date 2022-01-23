#include "cTransformComponent.h"

#include <process.h>
#include <Windows.h>

// Gems Midterm Q3
CRITICAL_SECTION CSTransformData;

cTransformComponent::cTransformComponent() :cComponent(), mPosition(glm::vec3(0.0f)), orientation(glm::vec3(0.0f)), scale(glm::vec3(1.0f)) {

	this->mType = "Transform";
	InitializeCriticalSection(&CSTransformData);

	return;
}

cTransformComponent::~cTransformComponent() {

	DeleteCriticalSection(&CSTransformData);

	return;
}

glm::vec3 cTransformComponent::getPosition() {

	EnterCriticalSection(&CSTransformData);

	glm::vec3 pos = this->mPosition;

	LeaveCriticalSection(&CSTransformData);

	return pos;
}

void cTransformComponent::setPosition(glm::vec3 position) {

	EnterCriticalSection(&CSTransformData);

	this->mPosition = position;

	LeaveCriticalSection(&CSTransformData);

	return;
}

void cTransformComponent::serializeComponent(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) {
	writer.StartObject();

	writer.String("componentType");
	writer.String(this->mType.c_str());

	glm::vec3 pos = this->getPosition();
	writer.String("position");
	writer.StartObject();
	writer.String("x");
	writer.Double(pos.x);
	writer.String("y");
	writer.Double(pos.y);
	writer.String("z");
	writer.Double(pos.z);
	writer.EndObject();

	glm::vec3 eulerOrientation = glm::degrees(glm::eulerAngles<float>(this->orientation));

	writer.String("orientation");
	writer.StartObject();
	writer.String("x");
	writer.Double(eulerOrientation.x);
	writer.String("y");
	writer.Double(eulerOrientation.y);
	writer.String("z");
	writer.Double(eulerOrientation.z);
	writer.EndObject();

	writer.String("scale");
	writer.StartObject();
	writer.String("x");
	writer.Double(this->scale.x);
	writer.String("y");
	writer.Double(this->scale.y);
	writer.String("z");
	writer.Double(this->scale.z);
	writer.EndObject();

	writer.EndObject();
}

glm::quat cTransformComponent::getQOrientation(void) { return this->orientation; }

void cTransformComponent::setQOrientation(glm::quat newOrientation) {

	this->orientation = newOrientation;

	return;
}

void cTransformComponent::setOrientationEulerAngles(glm::vec3 newAnglesEuler, bool bIsDegrees) {
	
	if (bIsDegrees) {
		newAnglesEuler = glm::vec3(glm::radians(newAnglesEuler.x), glm::radians(newAnglesEuler.y), glm::radians(newAnglesEuler.z));
	}

	this->orientation = glm::quat(glm::vec3(newAnglesEuler.x, newAnglesEuler.y, newAnglesEuler.z));
	return;
}

void cTransformComponent::setOrientationEulerAngles(float x, float y, float z, bool bIsDegrees) {
	this->setOrientationEulerAngles(glm::vec3(x, y, z), bIsDegrees);
}

void cTransformComponent::adjOrientationEulerAngles(glm::vec3 adjAngleEuler, bool bIsDegrees) {
	
	if (bIsDegrees) {
		adjAngleEuler = glm::vec3(glm::radians(adjAngleEuler.x), glm::radians(adjAngleEuler.y), glm::radians(adjAngleEuler.z));
	}

	// Step 1: make a quaternion that represents the angle we want to rotate
	glm::quat rotationAdjust(adjAngleEuler);

	// Step 2: Multiply this quaternion by the existing quaternion. This "adds" the angle we want.
	this->orientation *= rotationAdjust;
}

void cTransformComponent::adjOrientationEulerAngles(float x, float y, float z, bool bIsDegrees) {
	this->adjOrientationEulerAngles(glm::vec3(x, y, z), bIsDegrees);
}

void cTransformComponent::adjMeshOrientationQ(glm::quat adjOrientQ) {
	this->orientation *= adjOrientQ;
}

void cTransformComponent::setUniformScale(float scale) {

	this->scale = glm::vec3(scale, scale, scale);

	return;
}

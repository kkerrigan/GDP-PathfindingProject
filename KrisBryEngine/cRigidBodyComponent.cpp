/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#include "cRigidBodyComponent.h"

cRigidBodyComponent::cRigidBodyComponent() : cComponent() {

	this->mRigidBody = nullptr;
	this->mType = "Rigid Body";

	return;
}

cRigidBodyComponent::~cRigidBodyComponent() {

	delete this->mRigidBody;
	this->mRigidBody = nullptr;

	return;
}

nPhysics::iRigidBody* cRigidBodyComponent::getRigidBody() {

	return this->mRigidBody;
}

void cRigidBodyComponent::setRigidBody(nPhysics::iRigidBody* pBody) {

	this->mRigidBody = pBody;
}

nPhysics::iShape* cRigidBodyComponent::getShape() {

	return this->mRigidBody->getShape();
}

void cRigidBodyComponent::serializeComponent(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) {

	//Start Component
	writer.StartObject();

	writer.String("componentType");
	writer.String(this->mType.c_str());

	writer.String(("position"));
	writer.StartObject();
	
	glm::vec3 pos;
	this->mRigidBody->getPosition(pos);
	writer.String("x");
	writer.Double(pos.x);
	writer.String("y");
	writer.Double(pos.y);
	writer.String("z");
	writer.Double(pos.z);
	writer.EndObject();

	writer.String(("orientation"));
	writer.StartObject();

	glm::quat ori;
	this->mRigidBody->getOrientation(ori);
	glm::vec3 eulerOri = glm::eulerAngles(ori);

	writer.String("x");
	writer.Double(eulerOri.x);
	writer.String("y");
	writer.Double(eulerOri.y);
	writer.String("z");
	writer.Double(eulerOri.z);
	writer.EndObject();

	writer.String(("acceleration"));
	writer.StartObject();

	glm::vec3 accel;
	this->mRigidBody->getAcceleration(accel);
	writer.String("x");
	writer.Double(accel.x);
	writer.String("y");
	writer.Double(accel.y);
	writer.String("z");
	writer.Double(accel.z);
	writer.EndObject();

	writer.String(("velocity"));
	writer.StartObject();

	glm::vec3 vel;
	this->mRigidBody->getVelocity(vel);
	writer.String("x");
	writer.Double(vel.x);
	writer.String("y");
	writer.Double(vel.y);
	writer.String("z");
	writer.Double(vel.z);
	writer.EndObject();


	return;
}
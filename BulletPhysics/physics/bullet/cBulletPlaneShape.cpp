#include "cBulletPlaneShape.h"
#include "nConvert.h"

nPhysics::cBulletPlaneShape::cBulletPlaneShape(const glm::vec3& normal, const float constant) {

	// HACK: not sure why all the normals need to be the same....
	this->mShape = new btStaticPlaneShape(nConvert::toBullet(glm::vec3(0.0f, 1.0f, 0.0f)), nConvert::toBullet(0.0f));

	return;
}

nPhysics::cBulletPlaneShape::~cBulletPlaneShape() {

	delete this->mShape;
	this->mShape = nullptr;

	return;
}

float nPhysics::cBulletPlaneShape::getPlaneConstant() {
	
	return nConvert::toGlm(this->mShape->getPlaneConstant());
}

glm::vec3 nPhysics::cBulletPlaneShape::getPlaneNormal() {
	
	return nConvert::toGlm(this->mShape->getPlaneNormal());;
}

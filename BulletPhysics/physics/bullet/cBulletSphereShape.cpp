#include "cBulletSphereShape.h"
#include "nConvert.h"

nPhysics::cBulletSphereShape::cBulletSphereShape(float radius) {

	this->mShape = new btSphereShape(btScalar(radius));

	return;
}

nPhysics::cBulletSphereShape::~cBulletSphereShape() {

	delete this->mShape;

	return;
}

float nPhysics::cBulletSphereShape::getRadius() {
	
	return nConvert::toBullet(static_cast<btSphereShape*>(this->mShape)->getRadius());
}

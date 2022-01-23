/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#include "cPhysicsFactory.h"

#include "cPhysicsWorld.h"
#include "cSphereShape.h"
#include "cPlaneShape.h"
#include "cRigidBody.h"
#include "cSoftBody.h"

nPhysics::iPhysicsWorld* nPhysics::cPhysicsFactory::createPhysicsWorld() {
	
	return new nPhysics::cPhysicsWorld();
}

nPhysics::iRigidBody* nPhysics::cPhysicsFactory::createRigidBody(const sRigidBodyDef& def, iShape* shape) {
	
	return new cRigidBody(def, shape);
}

nPhysics::iSphereShape* nPhysics::cPhysicsFactory::createSphere(float radius) {
	
	return new cSphereShape(radius);
}

nPhysics::iPlaneShape* nPhysics::cPhysicsFactory::createPlane(const glm::vec3& normal, float constant) {
	
	return new cPlaneShape(normal, constant);
}

nPhysics::iSoftBody* nPhysics::cPhysicsFactory::createSoftBody(const sSoftBodyDef& def) {
	
	return new cSoftBody(def);
}

DLL_EXPORT nPhysics::iPhysicsFactory* createPhysicsFactory() {

	return new nPhysics::cPhysicsFactory();
}

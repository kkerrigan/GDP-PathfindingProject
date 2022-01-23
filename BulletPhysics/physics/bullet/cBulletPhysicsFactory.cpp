#include "cBulletPhysicsFactory.h"
#include "cBulletPhysicsWorld.h"
#include "cBulletRigidBody.h"
#include "cBulletSphereShape.h"
#include "cBulletPlaneShape.h"
#include "nConvert.h"

nPhysics::iPhysicsWorld* nPhysics::cBulletPhysicsFactory::createPhysicsWorld() {
	
	return new nPhysics::cBulletPhysicsWorld();
}

nPhysics::iRigidBody* nPhysics::cBulletPhysicsFactory::createRigidBody(const sRigidBodyDef& def, iShape* shape) {
	
	btRigidBody* bulletBody = nullptr;

	if (shape->getShapeType() == nPhysics::SHAPE_TYPE_SPHERE) {

		cBulletSphereShape* sphere = static_cast<cBulletSphereShape*>(shape);

		btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(nConvert::toBullet(nConvert::quaternionFromEuler(def.orientation)),
																					nConvert::toBullet(def.position)));

		btScalar mass = def.mass;
		bool isDynamic = (mass != 0.0f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic) {
			sphere->getShape()->calculateLocalInertia(mass, localInertia);
		}

		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, sphere->getShape(), localInertia);
		rbInfo.m_restitution = 1.0f;
		bulletBody = new btRigidBody(rbInfo);
	}
	else if (shape->getShapeType() == nPhysics::SHAPE_TYPE_PLANE) {

		cBulletPlaneShape* plane = static_cast<cBulletPlaneShape*>(shape);

		btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(nConvert::toBullet(nConvert::quaternionFromEuler(def.orientation)),
			nConvert::toBullet(def.position)));

		btRigidBody::btRigidBodyConstructionInfo rbInfo(0, motionState, plane->getShape(), btVector3(0, 0, 0));
		rbInfo.m_restitution = 0.5f;

		bulletBody = new btRigidBody(rbInfo);
	}

	cBulletRigidBody* rigidBody = new cBulletRigidBody(bulletBody, shape);
	return rigidBody;
}

nPhysics::iSphereShape* nPhysics::cBulletPhysicsFactory::createSphere(float radius) {
	
	return new cBulletSphereShape(radius);
}

nPhysics::iPlaneShape* nPhysics::cBulletPhysicsFactory::createPlane(const glm::vec3& normal, float constant) {
	
	return new cBulletPlaneShape(normal, constant);
}

DLL_EXPORT nPhysics::iPhysicsFactory* createPhysicsFactory() {

	return new nPhysics::cBulletPhysicsFactory();
}

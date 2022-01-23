#include "cBulletPhysicsWorld.h"
#include "cBulletRigidBody.h"
#include "nConvert.h"

nPhysics::cBulletPhysicsWorld::cBulletPhysicsWorld() {

	this->mCollisionConfiguration = new btDefaultCollisionConfiguration();
	this->mDispatcher = new btCollisionDispatcher(this->mCollisionConfiguration);
	this->mOverlappingPairCache = new btDbvtBroadphase();
	this->mSolver = new btSequentialImpulseConstraintSolver;
	this->mDynamicsWorld = new btDiscreteDynamicsWorld(this->mDispatcher, this->mOverlappingPairCache,
		this->mSolver, this->mCollisionConfiguration);

	this->setGravity(glm::vec3(0.0f, -2.81f, 0.0f));

	return;
}

nPhysics::cBulletPhysicsWorld::~cBulletPhysicsWorld() {

	//cleanup the physics objects
	for (int i = this->mDynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--) {

		btCollisionObject* obj = this->mDynamicsWorld->getCollisionObjectArray()[i];
		
		btRigidBody * body = btRigidBody::upcast(obj);
		if (body && body->getMotionState()) {
			delete body->getMotionState();
		}
		
		this->mDynamicsWorld->removeCollisionObject(obj);
		delete obj;

		for (int j = 0; j < this->mCollisionShapes.size(); j++) {
			
			btCollisionShape* shape = this->mCollisionShapes[j];
			this->mCollisionShapes[j] = nullptr;
			delete shape;
		}

		//delete dynamics world
		delete this->mDynamicsWorld;

		//delete solver
		delete this->mSolver;

		//delete broad phase
		delete this->mOverlappingPairCache;

		//delete dispatcher
		delete this->mDispatcher;

		//delete configuration
		delete this->mCollisionConfiguration;

		//clear the vector of shapes
		this->mCollisionShapes.clear();
	}

	return;
}

void nPhysics::cBulletPhysicsWorld::setGravity(const glm::vec3& grav) {

	this->mDynamicsWorld->setGravity(nConvert::toBullet(grav));

	return;
}

bool nPhysics::cBulletPhysicsWorld::addRigidBody(iRigidBody* pBody) {

	cBulletRigidBody* bulletBody = static_cast<cBulletRigidBody*>(pBody);
	if (!bulletBody) return false;

	mDynamicsWorld->addRigidBody(bulletBody->getBulletBody());

	return true;
}

bool nPhysics::cBulletPhysicsWorld::removeRigidBody(iRigidBody* pBody) {

	cBulletRigidBody* bulletBody = static_cast<cBulletRigidBody*>(pBody);
	if (!bulletBody) return false;

	btRigidBody* bulletRigidBody = bulletBody->getBulletBody();

	mDynamicsWorld->removeRigidBody(bulletRigidBody);

	return true;
}

void nPhysics::cBulletPhysicsWorld::update(float deltaTime) {

	this->mDynamicsWorld->stepSimulation(deltaTime, 0, 1.f / 60.f);

	return;
}

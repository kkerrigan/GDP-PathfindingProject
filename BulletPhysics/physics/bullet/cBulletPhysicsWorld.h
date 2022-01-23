/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#ifndef _cBulletPhysicsWorld_HG_
#define _cBulletPhysicsWorld_HG_

#include <physics/interfaces/iPhysicsWorld.h>
#include <btBulletDynamicsCommon.h>

namespace nPhysics {

	class cBulletPhysicsWorld : public iPhysicsWorld {
	public:
		cBulletPhysicsWorld();
		virtual ~cBulletPhysicsWorld();

		virtual void setGravity(const glm::vec3& grav) override;

		virtual bool addRigidBody(iRigidBody* pBody) override;
		virtual bool removeRigidBody(iRigidBody* pBody) override;

		virtual void update(float deltaTime) override;

	private:
		//collision configuration contains default setup for memory, collision setup. 
		//Advanced users can create their own configuration.
		btDefaultCollisionConfiguration* mCollisionConfiguration;

		//use the default collision dispatcher. 
		//For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		btCollisionDispatcher* mDispatcher;

		//btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
		btBroadphaseInterface* mOverlappingPairCache;

		//the default constraint solver. 
		//For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
		btSequentialImpulseConstraintSolver* mSolver;

		btDiscreteDynamicsWorld* mDynamicsWorld;
		btAlignedObjectArray<btCollisionShape*> mCollisionShapes;

	};
}

#endif // !_cBulletPhysicsWold_HG_


/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#ifndef _cPhysicsWorld_HG_
#define _cPhysicsWorld_HG_

#include <vector>

#include <physics/interfaces/iPhysicsWorld.h>
#include <physics/interfaces/iCollisionBody.h>
#include <physics/interfaces/iRigidBody.h>
#include <physics/interfaces/iSoftBody.h>

#include "sRK4State.h"
#include "sCollisionDetails.h";

namespace nPhysics {

	class cPhysicsWorld : public iPhysicsWorld {
	public:

		virtual ~cPhysicsWorld();

		virtual void setGravity(const glm::vec3& grav) override;

		virtual bool addCollisionBody(iCollisionBody* pBody) override;
		virtual bool removeCollisionBody(iCollisionBody* pBody) override;

		virtual void update(float deltaTime) override;

	private:
		glm::vec3 mGravity;
		float mElaspedTime = 0.0f;
		std::vector<iCollisionBody*> mBodies;

		// Collision Detection
		bool mIsColliding(iRigidBody* bodyA, iRigidBody* bodyB, sCollisionDetails& collision, float deltaTime);
		void mCollideRigidBodySoftBody(iCollisionBody* bodyA, iCollisionBody* bodyB, float deltaTime);
		int mIntersectMovingSpherePlane(iRigidBody* sphere, glm::vec3 movementVector, iRigidBody* plane, float &acceleration, glm::vec3& q) const;
		int mIntersectMovingSpherePlane(iSoftBody* softBody, size_t nodeIndex, glm::vec3 movementVector, iRigidBody* plane, float& timestep, glm::vec3& q) const;
		int mIntersectMovingSphereSphere(iRigidBody* sphereA, iRigidBody* sphereB, float& acceleration) const;
		int mSphereIntersectionTest(glm::vec3 distance, glm::vec3 normalizedVelocity, float radiiSum, float& accel) const;

		// RK4 Integration
		Derivative mEvaluate(const State& initialState, glm::vec3 t, float deltaTime, const Derivative& d);
		void mIntegrate(State& state, glm::vec3 t, float deltaTime);

		// Collision response
		void mSpherePlaneCollisionResponse(sCollisionDetails& collision, float deltaTime);
		void mSphereSphereCollisionResponse(sCollisionDetails& collision, float deltaTime);

		// Axis aligned bounding box checks
		bool mAreAABBOverlaping(glm::vec3 boxAMin, glm::vec3 boxAMax, glm::vec3 boxBMin, glm::vec3 boxBMax);
		bool mBroadPhaseTest(iCollisionBody* bodyA, iCollisionBody* bodyB);
	};
}

#endif

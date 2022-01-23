/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#ifndef _cPhysicsFactory_HG_
#define _cPhysicsFactory_HG_

#include <physics/interfaces/iPhysicsFactory.h>

namespace nPhysics {

	class cPhysicsFactory : public iPhysicsFactory {
	public:

		virtual iPhysicsWorld* createPhysicsWorld() override;
		virtual iRigidBody* createRigidBody(const sRigidBodyDef& def, iShape* shape) override;
		virtual iSphereShape* createSphere(float radius) override;
		virtual iPlaneShape* createPlane(const glm::vec3& normal, float constant) override;
		virtual iSoftBody* createSoftBody(const sSoftBodyDef& def) override;
	};
}

#endif

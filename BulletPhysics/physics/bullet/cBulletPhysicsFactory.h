/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#ifndef _cBulletPhysicsFactory_HG_
#define _cBulletPhysicsFactory_HG_

#include <physics/interfaces/iPhysicsFactory.h>

namespace nPhysics {

	class cBulletPhysicsFactory : public iPhysicsFactory {
	public:
		virtual iPhysicsWorld* createPhysicsWorld() override;
		virtual iRigidBody* createRigidBody(const sRigidBodyDef& def, iShape* shape) override;
		virtual iSphereShape* createSphere(float radius) override;
		virtual iPlaneShape* createPlane(const glm::vec3& normal, float constant) override;
	};
}

#endif // !_cBulletPhysicsFactory_HG_


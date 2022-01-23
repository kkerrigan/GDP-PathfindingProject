/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#ifndef _iPhysicsFactory_HG_
#define _iPhysicsFactory_HG_

#define DLL_EXPORT extern "C" _declspec(dllexport)

#include <gameMath.h>

#include "iShape.h"
#include "iPhysicsWorld.h"
#include "iRigidBody.h"
#include "iSoftBody.h"
#include "sRigidBodyDef.h"
#include "sSoftBodyDef.h"
#include "iSphereShape.h"
#include "iPlaneShape.h"

namespace nPhysics {

	class iPhysicsFactory {
	public:
		virtual ~iPhysicsFactory() {}
		virtual iPhysicsWorld* createPhysicsWorld() = 0;
		virtual iRigidBody* createRigidBody(const sRigidBodyDef& def, iShape* shape) = 0;
		virtual iSphereShape* createSphere(float radius) = 0;
		virtual iPlaneShape* createPlane(const glm::vec3& normal, float constant) = 0;
		virtual iSoftBody* createSoftBody(const sSoftBodyDef& def) = 0;
	};

	DLL_EXPORT iPhysicsFactory* createPhysicsFactory();
	using fCreatePhysicsFactory = iPhysicsFactory* (*)();
}

#endif

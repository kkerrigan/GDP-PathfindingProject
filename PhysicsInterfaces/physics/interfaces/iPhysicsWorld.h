/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#ifndef _iPhysicsWorld_HG_
#define _iPhysicsWorld_HG_

#include <gameMath.h>
#include "iCollisionBody.h"

namespace nPhysics {

	class iPhysicsWorld {
	public:
		virtual ~iPhysicsWorld() {}

		virtual void setGravity(const glm::vec3& grav) = 0;

		virtual bool addCollisionBody(iCollisionBody* pBody) = 0;
		virtual bool removeCollisionBody(iCollisionBody* pBody) = 0;

		virtual void update(float deltaTime) = 0;
	};
}

#endif

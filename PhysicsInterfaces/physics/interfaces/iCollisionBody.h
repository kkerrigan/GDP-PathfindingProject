/**
 * @file iCollisionBody.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This class controls the behaviour of both rigid and soft bodies.
 */

#ifndef _iCollisionBody_HG_
#define _iCollisionBody_HG_

#include <gameMath.h>
#include "eBodyType.h"

namespace nPhysics {

	class iCollisionBody {
	public:
		virtual ~iCollisionBody() {}
		
		virtual eBodyType getBodyType() = 0;
		virtual void getAABB(glm::vec3& minOut, glm::vec3& maxOut) = 0;

	};
}

#endif // !_iCollisionBody_HG_


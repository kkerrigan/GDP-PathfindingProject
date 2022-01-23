/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#ifndef _sRigidBodyDef_HG_
#define _sRigidBodyDef_HG_

#include <gameMath.h>

namespace nPhysics {

	struct sRigidBodyDef {

		sRigidBodyDef()
			: mass(0.0f),
			  position(0.0f, 0.0f, 0.0f),
			  velocity(0.0f, 0.0f, 0.0f),
			  orientation(0.0f, 0.0f, 0.0f),
			  acceleration(0.0f, 0.0f, 0.0f)
		{}

		glm::vec3 position;
		glm::vec3 orientation; // Euler
		glm::vec3 velocity;
		glm::vec3 acceleration;
		 
		float mass;
	};
}

#endif

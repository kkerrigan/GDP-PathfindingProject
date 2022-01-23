/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#ifndef _sCollisionDetails_HG_
#define _sCollisionDetails_HG_

#include <gameMath.h>

#include <physics/interfaces/iRigidBody.h>

namespace nPhysics {

	struct sCollisionDetails {

		enum eCollisionType {
			SPHERE_PLANE,
			SPHERE_SPHERE,
			UNKNOWN
		};

		sCollisionDetails() {

			this->contactVelocityA = glm::vec3(0.0f);
			this->contactVelocityB = glm::vec3(0.0f);

			this->contactPosition = glm::vec3(0.0f);
			this->contactNormal = glm::vec3(0.0f);

			this->timeStep = 0.0f;

			this->bodyA = nullptr;
			this->bodyB = nullptr;

			this->collisionType = eCollisionType::UNKNOWN;

			return;
		}

		glm::vec3 contactVelocityA;
		glm::vec3 contactVelocityB;

		glm::vec3 contactPosition; // Only needed for SPHERE-PLANE
		glm::vec3 contactNormal;
		
		float timeStep;

		iRigidBody* bodyA;
		iRigidBody* bodyB;

		eCollisionType collisionType;
	};
}


#endif

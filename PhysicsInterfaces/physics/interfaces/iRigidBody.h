/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#ifndef _iRigidBody_HG_
#define _iRigidBody_HG_

#include <gameMath.h>
#include "iShape.h"
#include "iCollisionBody.h"

namespace nPhysics {

	class iRigidBody : public iCollisionBody {
	public:
		virtual ~iRigidBody() {}

		virtual void getPosition(glm::vec3& positionOut) = 0;
		virtual void setPosition(glm::vec3 position) = 0;

		virtual void getPreviousPosition(glm::vec3& positionOut) = 0;
		virtual void setPreviousPosition(glm::vec3 position) = 0;

		virtual void getOrientation(glm::quat& orientationOut) = 0;
		virtual void setOrientation(glm::quat orientation) = 0;

		virtual void getVelocity(glm::vec3& velocityOut) = 0;
		virtual void setVelocity(glm::vec3 velocity) = 0;

		virtual void getAcceleration(glm::vec3& accelerationOut) = 0;
		virtual void setAcceleration(glm::vec3 acceleration) = 0;

		virtual void getMass(float& massOut) = 0;
		virtual void setMass(float mass) = 0;

		virtual void applyFoce(glm::vec3 forceAccel) = 0;

		virtual iShape* getShape() = 0;
		virtual eBodyType getBodyType() override {

			return eBodyType::RIGID_BODY;
		}

	protected:
		iRigidBody() {}
		iRigidBody(const iRigidBody& copy) {}
		iRigidBody& operator=(const iRigidBody& copy) { return *this; }
	};
}

#endif

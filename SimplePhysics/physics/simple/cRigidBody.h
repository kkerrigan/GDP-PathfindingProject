/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#ifndef _cRigidBody_HG_
#define _cRigidBody_HG_

#include <physics/interfaces/iRigidBody.h>
#include <physics/interfaces/sRigidBodyDef.h>
#include <physics/interfaces/iShape.h>

namespace nPhysics {

	class cRigidBody : public iRigidBody {
	public:
		cRigidBody(const sRigidBodyDef& def, iShape* shape);
		virtual ~cRigidBody();

		virtual void getPosition(glm::vec3& positionOut) override;
		virtual void setPosition(glm::vec3 position) override;

		virtual void getPreviousPosition(glm::vec3& positionOut) override;
		virtual void setPreviousPosition(glm::vec3 position) override;

		virtual void getOrientation(glm::quat& orientationOut) override;
		virtual void setOrientation(glm::quat orientation) override;

		virtual void getVelocity(glm::vec3& velocityOut) override;
		virtual void setVelocity(glm::vec3 velocity) override;

		virtual void getAcceleration(glm::vec3& accelerationOut) override;
		virtual void setAcceleration(glm::vec3 acceleration) override;

		virtual void getMass(float& massOut) override;
		virtual void setMass(float mass) override;

		virtual void applyFoce(glm::vec3 forceAccel) override;

		virtual iShape* getShape() override;
		virtual void getAABB(glm::vec3& minOut, glm::vec3& maxOut) override;

	protected:
		cRigidBody();
		cRigidBody(const cRigidBody& copy);
		cRigidBody& operator=(const cRigidBody& copy);

	private:
		iShape* mShape;

		glm::vec3 mPosition;
		glm::vec3 mPreviousPosition;
		glm::vec3 mVelocity;
		glm::quat mOrientation;
		glm::vec3 mAcceleration;
		float mMass;

		void mSetOrientationEulerAngles(glm::vec3 newAnglesEuler, bool bIsDegrees);
	};
}

#endif

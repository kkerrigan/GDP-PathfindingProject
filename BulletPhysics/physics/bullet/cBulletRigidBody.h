/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#ifndef _cBulletRigidBody_HG_
#define _cBulletRigidBody_HG_

#include <physics/interfaces/iRigidBody.h>
#include <physics/interfaces/sRigidBodyDef.h>
#include <physics/interfaces/iShape.h>

#include <btBulletDynamicsCommon.h>

namespace nPhysics {

	class cBulletRigidBody : public iRigidBody {
	public:
		cBulletRigidBody(btRigidBody* bulletBody, iShape* shape);
		virtual ~cBulletRigidBody();

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

		inline btRigidBody* getBulletBody(){ return mBody; }

	protected:
		cBulletRigidBody();
		cBulletRigidBody(const cBulletRigidBody& copy);
		cBulletRigidBody& operator=(const cBulletRigidBody& copy);

	private:
		iShape* mShape;
		btRigidBody* mBody;
	};
}

#endif // !_cBulletRigidBody_HG_


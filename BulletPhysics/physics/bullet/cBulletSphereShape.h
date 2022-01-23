/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#ifndef _cBulletSphereShape_HG_
#define _cBulletSphereShape_HG_

#include <physics/interfaces/iSphereShape.h>
#include <btBulletDynamicsCommon.h>

namespace nPhysics {
	
	class cBulletSphereShape : public iSphereShape {
	public:
		cBulletSphereShape(float radius);
		virtual ~cBulletSphereShape();
		virtual float getRadius() override;

		inline btCollisionShape* getShape() {

			return this->mShape;
		}

	private:
		btCollisionShape* mShape;
	};

}

#endif // !_cBulletSphereShape_HG_


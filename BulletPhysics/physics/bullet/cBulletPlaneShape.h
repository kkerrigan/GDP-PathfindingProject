/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#ifndef _cBulletPlaneShape_HG_
#define _cBulletPlaneShape_HG_

#include <physics/interfaces/iPlaneShape.h>
#include <btBulletDynamicsCommon.h>

namespace nPhysics {

	class cBulletPlaneShape : public iPlaneShape {
	public:
		cBulletPlaneShape(const glm::vec3& normal, const float constant);
		virtual ~cBulletPlaneShape();

		virtual float getPlaneConstant() override;
		virtual glm::vec3 getPlaneNormal() override;

		inline btStaticPlaneShape* getShape() {

			return this->mShape;
		}

	private:
		btStaticPlaneShape* mShape;
	};
}

#endif // !_cBulletPlaneShape_HG_


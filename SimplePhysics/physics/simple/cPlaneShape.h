/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#ifndef _cPlaneShape_HG_
#define _cPlaneShape_HG_

#include <physics/interfaces/iPlaneShape.h>

namespace nPhysics {

	class cPlaneShape : public iPlaneShape {
	public:

		cPlaneShape(const glm::vec3& normal = glm::vec3(0.0f, 1.0f, 0.0f), const float constant = 0.0f);

		virtual float getPlaneConstant() override;
		virtual glm::vec3 getPlaneNormal() override;

	private:
		glm::vec3 mNormal;
		float mConstant;
	};
}

#endif

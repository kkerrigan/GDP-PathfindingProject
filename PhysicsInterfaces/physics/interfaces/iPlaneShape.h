/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#ifndef _iPlaneShape_HG_
#define _iPlaneShape_HG_

#include <gameMath.h>

#include "iShape.h"

namespace nPhysics {

	class iPlaneShape : public iShape {
	public:
		virtual ~iPlaneShape() {}
		virtual float getPlaneConstant() = 0;
		virtual glm::vec3 getPlaneNormal() = 0;

	protected:
		iPlaneShape() : iShape(eShapeType::SHAPE_TYPE_PLANE) {}
		iPlaneShape(const iPlaneShape& copy) : iShape(copy) {}
		iPlaneShape& operator=(const iPlaneShape& copy) { return *this; }
	};
}

#endif

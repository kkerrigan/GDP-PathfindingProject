/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#ifndef _iSphereShape_HG_
#define _iSphereShape_HG_

#include "iShape.h"

namespace nPhysics {

	class iSphereShape : public iShape {
	public:
		virtual ~iSphereShape() {}
		virtual float getRadius() = 0;

	protected:
		iSphereShape() : iShape(eShapeType::SHAPE_TYPE_SPHERE) {}
		iSphereShape(const iSphereShape& copy) : iShape(copy) {}
		iSphereShape& operator=(const iSphereShape& copy) { return *this; }
	};
}

#endif

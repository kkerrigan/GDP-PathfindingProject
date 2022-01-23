/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#ifndef _iShape_HG_
#define _iShape_HG_

#include "shapeTypes.h"

namespace nPhysics {

	class iShape {
	public:
		virtual ~iShape(){}

		inline eShapeType getShapeType() const { return this->mShapeType; }

	protected:
		iShape(eShapeType shapeType) : mShapeType(shapeType) {}
		iShape(const iShape& copy) {}
		iShape& operator=(const iShape& copy) { return *this; }

	private:
		eShapeType mShapeType;
	};
}

#endif

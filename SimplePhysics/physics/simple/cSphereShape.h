/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#ifndef _cSphereShape_HG_
#define _cSphereShape_HG_

#include <physics/interfaces/iSphereShape.h>

namespace nPhysics {

	class cSphereShape : public iSphereShape {
	public:
		cSphereShape(const float radius = 1.0f);

		virtual float getRadius() override;

	private:
		float mRadius;
	};
}

#endif

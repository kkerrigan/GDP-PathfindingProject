/**
 * @file cClothComponent.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This component will simulate a cloth using soft body objects.
 */

#ifndef _cClothComponent_HG_
#define _cClothComponent_HG_

#include <string>

#include <physics/interfaces/iSoftBody.h>
#include <physics/interfaces/sSoftBodyDef.h>

#include "cComponent.h"

class cClothComponent : public cComponent {
public:
	cClothComponent();
	virtual ~cClothComponent();

	nPhysics::iSoftBody* getSoftBody();
	void setSoftBody(nPhysics::iSoftBody* pBody);

private:
	std::string mType;
	nPhysics::iSoftBody* mSoftBody;
};


#endif // !_cClothComponent_HG_


/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#ifndef _cRigidBodyComponent_HG_
#define _ccRigidBodyComponent_HG_

#include <string>

#include <physics/interfaces/iRigidBody.h>
#include <physics/interfaces/sRigidBodyDef.h>

#include "cComponent.h"

#include <rapidjson/prettywriter.h>

class cRigidBodyComponent : public cComponent {
public:
	cRigidBodyComponent();
	virtual ~cRigidBodyComponent();

	nPhysics::iRigidBody* getRigidBody();
	void setRigidBody(nPhysics::iRigidBody* pBody);

	nPhysics::iShape* getShape();

	void serializeComponent(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);

private:
	nPhysics::iRigidBody* mRigidBody;
	std::string mType;
};

#endif

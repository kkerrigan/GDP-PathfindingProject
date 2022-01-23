#ifndef _cCollider_HG_
#define _cCollider_HG_

#include "cComponent.h"
#include <vector>
#include <gameMath.h>

class cCollider : public cComponent {
public:

	struct sPhysicsObject
	{
		glm::vec3 offsetFromOrigin;

		enum eShape
		{
			UNKOWN_SHAPE,
			SPHERE,
			TRIANGLE,
			AABB,
			AOB
		};

		void* theShape;
		eShape shape;
	};

	std::vector<sPhysicsObject> physics_objects;

	cCollider() : cComponent() {}
	~cCollider() {}
};

#endif

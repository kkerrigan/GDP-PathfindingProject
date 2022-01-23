/**
 * @file sSoftBodyDef.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This structure defines the elements of a soft body object.
 */

#ifndef _sSoftBodyDef_HG_
#define _sSoftBodyDef_HG_

#include <gameMath.h>
#include <vector>

namespace nPhysics {

	struct sSoftBodyNodeDef {

		sSoftBodyNodeDef(const glm::vec3& position, float mass)
			: position(position), mass(mass) {}
		
		glm::vec3 position;
		float mass;
	};

	struct sSoftBodyDef {

		float springConstant;
		float dampingFactor;
		float minWindForce;
		float maxWindForce;
		glm::vec3 windDirection;
		std::vector<sSoftBodyNodeDef> nodes;
		std::vector<std::pair<size_t, size_t>> springs;
	};
}

#endif // !_sSoftBodyDef_HG_

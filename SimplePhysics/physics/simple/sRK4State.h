/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#ifndef _sRK4State_HG_
#define _sRK4State_HG_

#include <gameMath.h>

namespace nPhysics {

	struct State {

		glm::vec3 position;
		glm::vec3 velocity;

		State() {

			this->position = glm::vec3(0.0f);
			this->velocity = glm::vec3(0.0f);

			return;
		}
	};

	struct Derivative {

		glm::vec3 dx; // dx/delta time = velocity
		glm::vec3 dv; // dv/delta time = acceleration

		Derivative() {

			this->dx = glm::vec3(0.0f);
			this->dv = glm::vec3(0.0f);

			return;
		}
	};
}


#endif

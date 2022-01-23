/**
 * @file iSoftBody.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This interface defines the behaviour of individual soft bodies.
 */

#ifndef _iSoftBody_HG_
#define _iSoftBody_HG_

#include <gameMath.h>
#include "iCollisionBody.h"

namespace nPhysics {

	class iSoftBody : public iCollisionBody {
	public:
		virtual ~iSoftBody() {}
		virtual void getNodePosition(size_t index, glm::vec3& positionOut) = 0;
		virtual void setNodePosition(size_t index, glm::vec3 newPosition) = 0;
		virtual void getNodePreviousPosition(size_t index, glm::vec3& prevPosOut) = 0;
		virtual void getNodeVelocity(size_t index, glm::vec3& velocity) = 0;
		virtual void setNodeVelocity(size_t index, glm::vec3 newVelocity) = 0;
		virtual void getNodeRadius(size_t index, float& floatOut) = 0;
		virtual size_t numNodes() = 0;
		virtual eBodyType getBodyType() override {

			return eBodyType::SOFT_BODY;
		}

		virtual void getNodeInAABB(size_t index, glm::vec3& minOut, glm::vec3& maxOut) = 0;
		virtual void setWindForce(glm::vec3 windDirection, glm::vec3 maxDistance) = 0;

	protected:
		iSoftBody() {}
		iSoftBody(const iSoftBody& copy) {}
		iSoftBody& operator=(const iSoftBody& copy) { return *this; }
	};
}


#endif // !_iSoftBody_HG_


/**
 * @file cSoftBody.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This class implements the iSoftBody interface and allows soft body objects to be created.
 */

#ifndef _cSoftBody_HG_
#define _cSoftBody_HG_

#include <gameMath.h>
#include <physics/interfaces/iSoftBody.h>
#include <physics/interfaces/sSoftBodyDef.h>

namespace nPhysics {

	class cPhysicsWorld;

	class cSoftBody : public iSoftBody {

		friend class cPhysicsWorld;

		class cNode;
		class cSpring {
		public:
			cSpring(cNode* nodeA, cNode* nodeB, float springConstant);

			void applyForceToNode();
			inline cNode* getNeighbour(cNode* node) {

				return node == this->nodeA ? this->nodeB : this->nodeA;
			}

			cNode* nodeA;
			cNode* nodeB;
			float restingDistance;
			float springConstant;
		};

		class cNode {
		public:
			cNode(const glm::vec3& position, float mass, float damping);

			inline bool isFixed() { return this->mass == 0.0f; }
			void integrate(float deltaTime);
			bool hasNeighbor(cNode* other);
			void calculateRadius();

			glm::vec3 position;
			glm::vec3 previousPosition;
			glm::vec3 velocity;
			glm::vec3 springForce;
			float radius;
			float mass;
			float dampingFactor;
			std::vector<cSpring*> springs;
		};

	public:
		cSoftBody(const sSoftBodyDef& def);
		virtual ~cSoftBody();

		virtual void getNodePosition(size_t index, glm::vec3& positionOut) override;
		virtual void setNodePosition(size_t index, glm::vec3 newPosition) override;
		virtual void getNodePreviousPosition(size_t index, glm::vec3& prevPosOut) override;
		virtual void getNodeVelocity(size_t index, glm::vec3& velocity) override;
		virtual void setNodeVelocity(size_t index, glm::vec3 newVelocity) override;
		virtual size_t numNodes() override;
		virtual void getNodeRadius(size_t index, float& floatOut) override;
		virtual void getNodeInAABB(size_t index, glm::vec3& minOut, glm::vec3& maxOut) override;
		virtual void getAABB(glm::vec3& minOut, glm::vec3& maxOut) override;
		virtual void setWindForce(glm::vec3 windDirection, glm::vec3 maxDistance) override;

		void internalUpdate(float deltaTime, const glm::vec3& gravity); // This is a time step

	protected:
		cSoftBody() : iSoftBody() {}
		cSoftBody(const cSoftBody& copy) : iSoftBody(copy) {}
		cSoftBody& operator=(const cSoftBody& copy) { return *this; }

	private:
		float mMinWindForce;
		float mMaxWindForce;
		glm::vec3 mMinWindDirection;
		glm::vec3 mMaxWindDirection;
		std::vector<cNode*> mNodes;
		std::vector<cSpring*> mSprings;

		void mCollideNodes();
		void mApplyWindForceToNode(cNode* currentNode, float deltaTime);
		bool mAreAABBOverlapingInternal(glm::vec3 boxAMin, glm::vec3 boxAMax, glm::vec3 boxBMin, glm::vec3 boxBMax);
		int mInternalSphereIntersectionTest(glm::vec3 distance, glm::vec3 normalizedVelocity, float radiiSum, float& accel) const;
	};
}

#endif // !_cSoftBody_HG_

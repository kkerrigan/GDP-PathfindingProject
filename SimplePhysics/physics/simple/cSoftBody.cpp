/**
 * @file cSoftBody.cpp
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Implements the cSoftBody class.
 */

#include <algorithm>

#include "cSoftBody.h"

nPhysics::cSoftBody::cSoftBody(const sSoftBodyDef& def) {

	this->mNodes.resize(def.nodes.size());
	for (size_t index = 0; index < def.nodes.size(); ++index) {

		this->mNodes[index] = new cNode(def.nodes[index].position, def.nodes[index].mass, def.dampingFactor);
	}

	for (size_t index = 0; index < def.springs.size(); ++index) {

		cNode* nodeA = this->mNodes[def.springs[index].first];
		cNode* nodeB = this->mNodes[def.springs[index].second];

		// Check to make sure we aren't adding more than one spring to a node.
		if (!nodeA->hasNeighbor(nodeB)) {

			cSpring* spring = new cSpring(nodeA, nodeB, def.springConstant);
			this->mSprings.push_back(spring);
			nodeA->springs.push_back(spring);
			nodeB->springs.push_back(spring);
		}
	}

	for (size_t index = 0; index < def.nodes.size(); ++index) {

		this->mNodes[index]->calculateRadius();
	}

	// Set wind values from sSoftBodyDef
	this->mMinWindForce = def.minWindForce;
	this->mMaxWindForce = def.maxWindForce;
	this->mMaxWindDirection = def.windDirection;
	this->mMinWindDirection = this->mMaxWindDirection;

	return;
}

nPhysics::cSoftBody::~cSoftBody() {

	for (size_t index = 0; index < this->mNodes.size(); ++index) {

		delete this->mNodes[index];
	}
	this->mNodes.clear();

	for (size_t index = 0; index < this->mSprings.size(); ++index) {

		delete this->mSprings[index];
	}
	this->mSprings.clear();

	return;
}

void nPhysics::cSoftBody::getNodePosition(size_t index, glm::vec3& positionOut) {

	positionOut = this->mNodes[index]->position;

	return;
}

void nPhysics::cSoftBody::setNodePosition(size_t index, glm::vec3 positionIn) {

	this->mNodes[index]->position = positionIn;

	return;
}

void nPhysics::cSoftBody::getNodePreviousPosition(size_t index, glm::vec3& prevPosOut) {

	prevPosOut = this->mNodes[index]->previousPosition;
}

void nPhysics::cSoftBody::getNodeVelocity(size_t index, glm::vec3& velocityOut) {

	velocityOut = this->mNodes[index]->velocity;

	return;
}

void nPhysics::cSoftBody::setNodeVelocity(size_t index, glm::vec3 velocityIn) {

	this->mNodes[index]->velocity = velocityIn;

	return;
}

size_t nPhysics::cSoftBody::numNodes() {
	
	return this->mNodes.size();
}

void nPhysics::cSoftBody::getNodeRadius(size_t index, float& floatOut) {

	floatOut = this->mNodes[index]->radius;

	return;
}

void nPhysics::cSoftBody::getNodeInAABB(size_t index, glm::vec3& minOut, glm::vec3& maxOut) {

	// Since we know our soft bodies are spheres, we can use the radius and position
	// to calculate the min and max extent
	float radius = 0.0f;
	glm::vec3 position(0.0f);

	this->getNodeRadius(index, radius);
	this->getNodePosition(index, position);

	minOut = position - glm::vec3(radius);
	maxOut = position + glm::vec3(radius);

	return;
}

void nPhysics::cSoftBody::getAABB(glm::vec3& minOut, glm::vec3& maxOut) {

	// Loop over the nodes to find the extent of the soft body
	for (size_t index = 0; index < this->mNodes.size(); ++index) {

		glm::vec3 min(0.0f);
		glm::vec3 max(0.0f);
		this->getNodeInAABB(index, min, max);

		// Do minimum check first
		if (min.x < minOut.x) { minOut.x = min.x; }
		if (min.y < minOut.y) { minOut.y = min.y; }
		if (min.z < minOut.z) { minOut.z = min.z; }

		// Now the maximum checks
		if (max.x > maxOut.x) { maxOut.x = max.x; }
		if (max.y > maxOut.y) { maxOut.y = max.y; }
		if (max.z > maxOut.z) { maxOut.z = max.z; }
	}

	return;
}

void nPhysics::cSoftBody::setWindForce(glm::vec3 windDirection, glm::vec3 maxDistance) {

	this->mMinWindDirection = windDirection;
	this->mMaxWindDirection = maxDistance;

	return;
}

// Called inside physics world
void nPhysics::cSoftBody::internalUpdate(float deltaTime, const glm::vec3& gravity) {

	for (size_t index = 0; index < this->mNodes.size(); ++index) {

		// Apply the wind force BEFORE the spring force
		this->mApplyWindForceToNode(this->mNodes[index], deltaTime);

		this->mNodes[index]->springForce = gravity;
	}

	for (size_t index = 0; index < this->mSprings.size(); ++index) {

		this->mSprings[index]->applyForceToNode();
	}

	for (size_t index = 0; index < this->mNodes.size(); ++index) {

		this->mNodes[index]->integrate(deltaTime);
	}

	// Collide with each other
	this->mCollideNodes();

	return;
}

void nPhysics::cSoftBody::mCollideNodes() {

	for (size_t idxA = 0; idxA < this->mNodes.size(); idxA++) {

		for (size_t idxB = idxA + 1; idxB < this->mNodes.size(); idxB++) {

			// Get the extents of each node and do a broadphase check
			cNode* nodeA = this->mNodes[idxA];
			cNode* nodeB = this->mNodes[idxB];

			glm::vec3 nodeAMin(0.0f);
			glm::vec3 nodeAMax(0.0f);
			glm::vec3 nodeBMin(0.0f);
			glm::vec3 nodeBMax(0.0f);

			this->getNodeInAABB(idxA, nodeAMin, nodeAMax);
			this->getNodeInAABB(idxB, nodeBMin, nodeBMax);

			// has neighbor check
			if (nodeA->hasNeighbor(nodeB)) continue;

			if (!this->mAreAABBOverlapingInternal(nodeAMin, nodeAMax, nodeBMin, nodeBMax)) {

				// Now check for collisions and displace the nodes if so
				glm::vec3 nodeAPos(0.0f);
				glm::vec3 nodeAVel(0.0f);
				glm::vec3 nodeBPos(0.0f);
				glm::vec3 nodeBVel(0.0f);
				float nodeARadius = 0.0f;
				float nodeBRadius = 0.0f;

				this->getNodePosition(idxA, nodeAPos);
				this->getNodeVelocity(idxA, nodeAVel);
				this->getNodeRadius(idxA, nodeARadius);
				
				this->getNodePosition(idxB, nodeBPos);
				this->getNodeVelocity(idxB, nodeBVel);
				this->getNodeRadius(idxB, nodeBRadius);

				glm::vec3 distance = nodeAPos - nodeBPos;
				glm::vec3 unitVector = glm::normalize(nodeAVel - nodeBVel);
				float radiiSum = nodeARadius + nodeBRadius;
				float t = 0.0f;

				if (this->mInternalSphereIntersectionTest(distance, unitVector, radiiSum, t) == 1) {

					// We have a wrinkle in the cloth (collision), better fix it!
					if (t > 0.0f && t < 0.0f) {

						// Swap node velocities
						this->setNodeVelocity(idxA, nodeBVel);
						this->setNodeVelocity(idxB, nodeAVel);
					}
				}
			} // if (!this->mAreAABBOverlapingInternal
		} // for (size_t idxB = idxA + 1; idxB < this->mNodes.size();
	} // for (size_t idxA = 0; idxA < this->mNodes.size();

	return;
}

void nPhysics::cSoftBody::mApplyWindForceToNode(cNode* currentNode, float deltaTime) {

	// To seem more realistic, each node should have a slightly different forced applied
	float randomDirection = (rand() % 50) / 50.0f;

	// Use glm::mix to create a direction vector somewhere between the min and max direction
	glm::vec3 direction = glm::mix(this->mMinWindDirection, this->mMaxWindDirection, randomDirection);

	// We won't apply the force to all nodes all the time so find a random number between 0-100%
	float windPercent = (rand() % 100) / 100.0f;
	if (windPercent < 0.65f) return; // No wind for this node this frame

	float windForce = (this->mMaxWindForce - this->mMinWindForce) * windPercent + this->mMinWindForce;
	// NOTE: No wind should be appiled to fixed nodes
	if (!currentNode->isFixed()) {

		currentNode->position += (direction * windForce * deltaTime);
	}

	return;
}

// Same behaviour as function in cPhysicsWorld, but used to check internal nodes overlap
bool nPhysics::cSoftBody::mAreAABBOverlapingInternal(glm::vec3 boxAMin, glm::vec3 boxAMax, glm::vec3 boxBMin, glm::vec3 boxBMax) {

	// Exit with no intersection if separated along an axis
	if (boxAMax.x < boxBMin.x || boxAMin.x > boxBMax.x) return false;
	if (boxAMax.y < boxBMin.y || boxAMin.y > boxBMax.y) return false;
	if (boxAMax.z < boxBMin.z || boxAMin.z > boxBMax.z) return false;

	// Overlapping on all areas means AABBs are intersecting
	return true;
}

// Same behaviour as function in cPhysicsWorld, but used to check internal node collisions
int nPhysics::cSoftBody::mInternalSphereIntersectionTest(glm::vec3 distance, glm::vec3 normalizedVelocity, float radiiSum, float& accel) const {

	float c = glm::dot(distance, distance) - radiiSum * radiiSum;

	if (c < 0.0f) {

		//spheres are overlapping so exit
		accel = 0.0f;
		return -1;
	}

	float a = glm::dot(normalizedVelocity, normalizedVelocity);
	if (a < glm::epsilon<float>()) {

		//not moving relative to each other
		return 0;
	}
	float b = glm::dot(normalizedVelocity, distance);
	if (b >= 0.0f) {

		//moving away from each other
		return 0;
	}

	float d = b * b - a * c;
	if (d < 0.0f) {

		//they do not intersect
		return 0;
	}

	accel = (-b - glm::sqrt(d)) / a;
	return 1;
}

//************************ Start of Spring implementation ********************************//
nPhysics::cSoftBody::cSpring::cSpring(cNode* nodeA, cNode* nodeB, float springConstant) {

	this->nodeA = nodeA;
	this->nodeB = nodeB;
	this->springConstant = springConstant;
	this->restingDistance = glm::length(this->nodeA->position - this->nodeB->position);

	return;
}

void nPhysics::cSoftBody::cSpring::applyForceToNode() {

	glm::vec3 separationAtoB = this->nodeB->position - this->nodeA->position;
	float currentDistance = glm::length(separationAtoB);

	// Apply Hook's Law to the nodes
	glm::vec3 force = (separationAtoB / currentDistance) * (-this->springConstant * (currentDistance - this->restingDistance));
	this->nodeB->springForce += force;
	this->nodeA->springForce -= force;

	return;
}

//************************ Start of Node implementation ********************************//
nPhysics::cSoftBody::cNode::cNode(const glm::vec3& position, float mass, float damping) {

	this->position = position;
	this->previousPosition = position;
	this->velocity = glm::vec3(0.0f);
	this->springForce = glm::vec3(0.0f);
	this->mass = mass;
	this->dampingFactor = damping;

	return;
}

void nPhysics::cSoftBody::cNode::integrate(float deltaTime)	{

	// If the node is fixed, do nothing
	if (this->isFixed()) return;

	// Otherwise, integrate.
	this->previousPosition = this->position;
	this->velocity += this->springForce * (deltaTime / this->mass);
	this->position += this->velocity * deltaTime;

	// Apply damping
	this->velocity *= glm::pow(1 - this->dampingFactor, deltaTime);

	return;
}

bool nPhysics::cSoftBody::cNode::hasNeighbor(cNode* other) {
	
	std::vector<cSpring*>::iterator it = std::find_if(this->springs.begin(), this->springs.end(), [other, this](cSpring* spring) {

		return other == spring->getNeighbour(this);
	});

	return it != this->springs.end();
}

void nPhysics::cSoftBody::cNode::calculateRadius() {

	float closestMember = std::numeric_limits<float>::max();

	for (size_t index = 0; index < springs.size(); ++index) {

		float distance = glm::length(springs[index]->getNeighbour(this)->position - position);
		closestMember = glm::min(closestMember, distance);
	}

	this->radius = 0.5f * closestMember;

	return;
}

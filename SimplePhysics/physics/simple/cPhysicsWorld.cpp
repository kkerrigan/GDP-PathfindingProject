/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 *
 * @section DESCRIPTION
 *
 * Implements the cPhysicsWorld class, which handles the interaction of all objects in the world.
 */

#include <iostream>

#include <physics/interfaces/iSphereShape.h>
#include <physics/interfaces/iPlaneShape.h>

#include <gameMath.h>

#include "cPhysicsWorld.h"
#include "cSoftBody.h"
#include "cRigidBody.h"
#include <algorithm> // for deleting

nPhysics::cPhysicsWorld::~cPhysicsWorld() {

	return;
}

void nPhysics::cPhysicsWorld::setGravity(const glm::vec3& grav) {

	this->mGravity = grav;

	return;
}

bool nPhysics::cPhysicsWorld::addCollisionBody(iCollisionBody* pBody) {

	if (pBody == nullptr) return false;

	// Check to see if it has already been added
	std::vector<iCollisionBody*>::iterator it = std::find(this->mBodies.begin(), this->mBodies.end(), pBody);
	if (it != this->mBodies.end()) return false; // Already exists

	this->mBodies.push_back(pBody);
	return true;
}

bool nPhysics::cPhysicsWorld::removeCollisionBody(iCollisionBody* pBody) {

	if (!pBody) return false;
	std::vector<iCollisionBody*>::iterator it = std::remove(this->mBodies.begin(), this->mBodies.end(), pBody);

	if (it == this->mBodies.end()) return false; // Nothing was removed
	this->mBodies.erase(it, this->mBodies.end());

	return true;
}

void nPhysics::cPhysicsWorld::update(float deltaTime) {

	this->mElaspedTime += deltaTime;

	// STEP 1: Intergrate RK4 - Rigid Bodies
	size_t numBodies = mBodies.size();
	for (size_t index = 0; index < numBodies; ++index) {

		nPhysics::iCollisionBody* currBody = this->mBodies[index];

		if (currBody->getBodyType() == eBodyType::RIGID_BODY) {

			nPhysics::iRigidBody* rigidBody = dynamic_cast<nPhysics::iRigidBody*>(currBody);

			// Integrate spheres
			if (rigidBody->getShape()->getShapeType() == nPhysics::eShapeType::SHAPE_TYPE_SPHERE) {

				// get current position / velocity and store in State object
				glm::vec3 curPos;
				glm::vec3 curVel;

				rigidBody->getPosition(curPos);
				rigidBody->getVelocity(curVel);

				State currState;
				currState.position = curPos;
				currState.velocity = curVel;

				rigidBody->applyFoce(this->mGravity);

				glm::vec3 curAccel;
				rigidBody->getAcceleration(curAccel);

				this->mIntegrate(currState, curAccel, deltaTime);

				rigidBody->setVelocity(currState.velocity);
				rigidBody->setPosition(currState.position);

				// set acceleration to zero at the end of the frame
				rigidBody->setAcceleration(glm::vec3(0.0f));
			}


			// store the last position
			glm::vec3 lastPos;
			rigidBody->getPosition(lastPos);
			rigidBody->setPreviousPosition(lastPos);
		}
		else {

			// Integrate soft bodies
			nPhysics::cSoftBody* sb = dynamic_cast<nPhysics::cSoftBody*>(currBody);
			sb->internalUpdate(deltaTime, this->mGravity);
		}
	}

	// STEP 2: Collision detection
	std::vector<nPhysics::sCollisionDetails> collisions;
	iRigidBody* rigidBodyA = nullptr;
	iRigidBody* rigidBodyB = nullptr;

	for (size_t idxA = 0; idxA < numBodies - 1; ++idxA) {
		for (size_t idxB = idxA + 1; idxB < numBodies; ++idxB) {

			iCollisionBody* bodyA = this->mBodies[idxA];
			iCollisionBody* bodyB = this->mBodies[idxB];

			// Broadphase check
			if (!this->mBroadPhaseTest(bodyA, bodyB)) continue;

			// Rigid vs Rigid Detection
			if (bodyA->getBodyType() == eBodyType::RIGID_BODY && bodyB->getBodyType() == eBodyType::RIGID_BODY) {

				rigidBodyA = dynamic_cast<iRigidBody*>(bodyA);
				rigidBodyB = dynamic_cast<iRigidBody*>(bodyB);

				nPhysics::sCollisionDetails currCollision;

				if (this->mIsColliding(rigidBodyA, rigidBodyB, currCollision, deltaTime)) {

					collisions.push_back(currCollision);
				}
			} // End Rigid vs Rigid Detection

			// Rigid vs Soft Dection (Handled right away so no collision details
			this->mCollideRigidBodySoftBody(bodyA, bodyB, deltaTime);
		}
	}

	// STEP 3: Collision Response
	size_t numCollisions = collisions.size();
	for (size_t index = 0; index < numCollisions; ++index) {

		nPhysics::sCollisionDetails currCollision = collisions[index];

		if (currCollision.collisionType == nPhysics::sCollisionDetails::eCollisionType::UNKNOWN) {

			std::cout << "Unknown collision stored at index " << index << std::endl;
			continue;
		}

		// Sphere-Plane collisions
		if (currCollision.collisionType == nPhysics::sCollisionDetails::eCollisionType::SPHERE_PLANE) {

			this->mSpherePlaneCollisionResponse(currCollision, deltaTime);
		}

		// sphere-sphere collisions
		if (currCollision.collisionType == nPhysics::sCollisionDetails::eCollisionType::SPHERE_SPHERE) {

			this->mSphereSphereCollisionResponse(currCollision, deltaTime);
		}
	}

	return;
}

bool nPhysics::cPhysicsWorld::mIsColliding(iRigidBody* bodyA, iRigidBody* bodyB, nPhysics::sCollisionDetails& collision, float deltaTime) {

	float timestep;
	glm::vec3 q;

	if (bodyA->getShape()->getShapeType() == eShapeType::SHAPE_TYPE_SPHERE) {

		// Sphere-Plane collisions
		if (bodyB->getShape()->getShapeType() == eShapeType::SHAPE_TYPE_PLANE) {

			glm::vec3 currSpherePos;
			bodyA->getPosition(currSpherePos);

			glm::vec3 currSpherePrevPos;
			bodyA->getPreviousPosition(currSpherePrevPos);

			glm::vec3 currSphereVel;
			bodyA->getVelocity(currSphereVel);

			int result = this->mIntersectMovingSpherePlane(bodyA, (currSpherePos - currSpherePrevPos), bodyB, timestep, q);

			if (result == 1) {

				if (timestep > 0.0f && timestep < 1.0f) {

					glm::vec3 planeVel;
					bodyB->getVelocity(planeVel);

					collision.collisionType = nPhysics::sCollisionDetails::eCollisionType::SPHERE_PLANE;
					collision.contactPosition = q;
					collision.bodyA = bodyA;
					collision.bodyB = bodyB;
					collision.contactVelocityA = currSphereVel;
					collision.contactVelocityB = planeVel;
					collision.contactNormal = static_cast<iPlaneShape*>(bodyB->getShape())->getPlaneNormal();
					collision.timeStep = timestep;

					return true;
				}
			}

			if (result == -1) {

				// Set the sphere to its previous position and project it off the normal
				glm::vec3 normal = static_cast<iPlaneShape*>(bodyB->getShape())->getPlaneNormal();

				glm::vec3 newSpherePos;
				newSpherePos = currSpherePos + (normal * deltaTime);
				newSpherePos.y += static_cast<iSphereShape*>(bodyA->getShape())->getRadius();
				bodyA->setPosition(newSpherePos);
			}
		}

		// Sphere-Sphere collisions
		if (bodyB->getShape()->getShapeType() == eShapeType::SHAPE_TYPE_SPHERE) {

			int result = this->mIntersectMovingSphereSphere(bodyA, bodyB, timestep);

			if (result == 1) {

				if (timestep > 0.0f && timestep < 1.0f) {

					glm::vec3 currSpherePos;
					bodyA->getPosition(currSpherePos);

					glm::vec3 collideSpherePos;
					bodyB->getPosition(collideSpherePos);

					glm::vec3 currSphereVel;
					bodyA->getVelocity(currSphereVel);

					glm::vec3 collideSphereVel;
					bodyB->getVelocity(collideSphereVel);

					collision.collisionType = nPhysics::sCollisionDetails::eCollisionType::SPHERE_SPHERE;
					collision.bodyA = bodyA;
					collision.bodyB = bodyB;
					collision.contactVelocityA = currSphereVel;
					collision.contactVelocityB = collideSphereVel;
					collision.contactNormal = glm::normalize(collideSpherePos - currSpherePos);
					collision.timeStep = timestep;

					return true;
				}

				if (result == -1) {

					glm::vec3 bodyAPrevPos;
					glm::vec3 bodyBPrevPos;

					bodyA->getPreviousPosition(bodyAPrevPos);
					bodyB->getPreviousPosition(bodyBPrevPos);

					bodyA->setPosition(bodyAPrevPos);
					bodyB->setPosition(bodyBPrevPos);
				}
			}
		}
	}

	if (bodyB->getShape()->getShapeType() == eShapeType::SHAPE_TYPE_SPHERE) {

		// Sphere-Plane collisions
		if (bodyA->getShape()->getShapeType() == eShapeType::SHAPE_TYPE_PLANE) {

			glm::vec3 curSpherePos;
			bodyB->getPosition(curSpherePos);

			glm::vec3 curSpherePrevPos;
			bodyB->getPreviousPosition(curSpherePrevPos);

			glm::vec3 currSphereVel;
			bodyB->getVelocity(currSphereVel);

			glm::vec3 curSphereVel;
			bodyB->getVelocity(curSphereVel);

			int result = this->mIntersectMovingSpherePlane(bodyB, (curSpherePos - curSpherePrevPos), bodyA, timestep, q);

			if (result == 1) {

				if (timestep > 0.0f && timestep < 1.0f) {

					glm::vec3 planeVel;
					bodyA->getVelocity(planeVel);

					collision.collisionType = nPhysics::sCollisionDetails::eCollisionType::SPHERE_PLANE;
					collision.contactPosition = q;
					collision.bodyA = bodyB;
					collision.bodyB = bodyA;
					collision.contactVelocityA = curSphereVel;
					collision.contactVelocityB = planeVel;
					collision.contactNormal = static_cast<iPlaneShape*>(bodyA->getShape())->getPlaneNormal();
					collision.timeStep = timestep;

					return true;
				}
			}

			if (result == -1) {

				// Set the sphere to its previous position and project it off the normal
				glm::vec3 normal = static_cast<iPlaneShape*>(bodyA->getShape())->getPlaneNormal();

				glm::vec3 newSpherePos;
				//bodyB->getPreviousPosition(newSpherePos);
				newSpherePos = curSpherePos + (normal * deltaTime);
				newSpherePos.y += static_cast<iSphereShape*>(bodyB->getShape())->getRadius();
				bodyB->setPosition(newSpherePos);
			}
		}

		// Sphere-Sphere collisions
		if (bodyA->getShape()->getShapeType() == eShapeType::SHAPE_TYPE_SPHERE) {

			int result = this->mIntersectMovingSphereSphere(bodyB, bodyA, timestep);

			if (result == 1) {

				if (timestep > 0.0f && timestep < 1.0f) {

					glm::vec3 currSpherePos;
					bodyB->getPosition(currSpherePos);

					glm::vec3 collideSpherePos;
					bodyA->getPosition(collideSpherePos);

					glm::vec3 currSphereVel;
					bodyB->getVelocity(currSphereVel);

					glm::vec3 collideSphereVel;
					bodyA->getVelocity(collideSphereVel);

					collision.collisionType = nPhysics::sCollisionDetails::eCollisionType::SPHERE_SPHERE;
					collision.bodyA = bodyB;
					collision.bodyB = bodyA;
					collision.contactVelocityA = currSphereVel;
					collision.contactVelocityB = collideSphereVel;
					collision.contactNormal = glm::normalize(collideSpherePos - currSpherePos); //currSpherePos - collideSpherePos
					collision.timeStep = timestep;

					return true;
				}
			}

			if (result == -1) {

				glm::vec3 bodyAPrevPos;
				glm::vec3 bodyBPrevPos;

				bodyA->getPreviousPosition(bodyAPrevPos);
				bodyB->getPreviousPosition(bodyBPrevPos);

				bodyA->setPosition(bodyAPrevPos);
				bodyB->setPosition(bodyBPrevPos);
			}
		}
	}

	return false;
}

void nPhysics::cPhysicsWorld::mCollideRigidBodySoftBody(iCollisionBody* bodyA, iCollisionBody* bodyB, float deltaTime) {

	iRigidBody* rigidBody = nullptr;
	iSoftBody* softBody = nullptr;

	float timestep;
	glm::vec3 q;

	if ((bodyA->getBodyType() == eBodyType::RIGID_BODY && bodyB->getBodyType() == eBodyType::SOFT_BODY) ||
		(bodyA->getBodyType() == eBodyType::SOFT_BODY && bodyB->getBodyType() == eBodyType::RIGID_BODY)) {

		// Need to know which one is the Rigid and which is Soft
		if (dynamic_cast<iSoftBody*>(bodyA)) {

			softBody = static_cast<iSoftBody*>(bodyA);
			rigidBody = static_cast<iRigidBody*>(bodyB);
		}
		else {

			softBody = static_cast<iSoftBody*>(bodyB);
			rigidBody = static_cast<iRigidBody*>(bodyA);
		}

		// Get the extents of the rigid body
		glm::vec3 rigidMin(0.0f);
		glm::vec3 rigidMax(0.0f);
		rigidBody->getAABB(rigidMin, rigidMax);

		// Do another broad phase check to see which nodes of the cloth were affected
		for (size_t index = 0; index < softBody->numNodes(); ++index) {

			glm::vec3 nodeMin(0.0f);
			glm::vec3 nodeMax(0.0f);
			glm::vec3 nodePos(0.0f);
			float nodeRadius = 0.0f;
			softBody->getNodeInAABB(index, nodeMin, nodeMax);
			softBody->getNodePosition(index, nodePos);
			softBody->getNodeRadius(index, nodeRadius);

			// if the rigid body is not touching the AABBs of the cloth, continue
			if (!this->mAreAABBOverlaping(rigidMin, rigidMax, nodeMin, nodeMax)) continue;

			// Otherwise do a sphere-sphere collision
			if (rigidBody->getShape()->getShapeType() == eShapeType::SHAPE_TYPE_SPHERE) {

				iSphereShape* sphere = static_cast<iSphereShape*>(rigidBody->getShape());
				glm::vec3 rigidBodyPos(0.0f);
				glm::vec3 rigidBodyVel(0.0f);
				glm::vec3 nodeVel(0.0f);

				rigidBody->getPosition(rigidBodyPos);
				rigidBody->getVelocity(rigidBodyVel);
				softBody->getNodeVelocity(index, nodeVel);

				glm::vec3 distance = rigidBodyPos - nodePos;
				glm::vec3 velVector = glm::normalize(rigidBodyVel - nodeVel);

				
				float radiiSum = sphere->getRadius() + nodeRadius;
				float accel = 0.0f;

				// Do the collision check
				int collisionCheck = this->mSphereIntersectionTest(distance, velVector, radiiSum, accel);
				if (collisionCheck == 1 || collisionCheck == -1) {

					// You bet! Move the nodes out of the rigid bodies way
					if ((accel > 0.0f && accel < 1.0f) || collisionCheck == -1) {

						glm::vec3 nodeVel(0.0f);
						softBody->getNodeVelocity(index, nodeVel);

						nodeVel = rigidBodyVel;
						softBody->setNodeVelocity(index, nodeVel);
					}
				}
			}

			// Sphere-Plane collision (cloth against the ground)
			if (rigidBody->getShape()->getShapeType() == eShapeType::SHAPE_TYPE_PLANE) {

				// ACTUALLY do a collision test
				glm::vec3 prevPos(0.0f);
				glm::vec3 movementVec(0.0f);

				softBody->getNodePreviousPosition(index, prevPos);
				movementVec = (nodePos - prevPos);

				int collisionCheck = this->mIntersectMovingSpherePlane(softBody, index, movementVec, rigidBody, timestep, q);
				if (collisionCheck == 1 || collisionCheck == -1) {

					if ((timestep > 0.0f && timestep < 1.0f) || collisionCheck == -1) {
						
						iPlaneShape* plane = static_cast<iPlaneShape*>(rigidBody->getShape());

						// Set the node to its previous position and project it off the normal
						glm::vec3 normal = plane->getPlaneNormal();

						glm::vec3 newNodePos;
						newNodePos = nodePos + (normal * deltaTime);
						softBody->setNodePosition(index, newNodePos);
						softBody->setNodeVelocity(index, glm::vec3(0.0f));
					}
				}
			}
		}
	}

	return;
}

int nPhysics::cPhysicsWorld::mIntersectMovingSpherePlane(iRigidBody* sphere, glm::vec3 movementVector, iRigidBody* plane, float& timestep, glm::vec3& q) const {

	// Get the shapes
	nPhysics::iSphereShape* sphereShape = static_cast<nPhysics::iSphereShape*>(sphere->getShape());
	nPhysics::iPlaneShape* planeShape = static_cast<nPhysics::iPlaneShape*>(plane->getShape());

	// Compute distance of sphere center to plane
	glm::vec3 spherePos;
	glm::vec3 planePos;
	sphere->getPosition(spherePos);
	plane->getPosition(planePos);
	float distance = glm::dot(spherePos - planePos, planeShape->getPlaneNormal());
	if (glm::abs(distance) <= sphereShape->getRadius()) {
		// The sphere is already overlapping the plane
		timestep = 0.0f;
		q = spherePos;
		return -1;
	}

	glm::vec3 sphereVel;
	sphere->getVelocity(sphereVel);
	float denom = glm::dot(planeShape->getPlaneNormal(), glm::normalize(sphereVel));
	if (denom * distance >= 0.0f) {
		// No intersection
		return 0;
	}
	else {
		// Sphere is moving towards the plane
		// Use +radius in computations if sphere in front of plane, else -radius
		float radius = distance > 0.0f ? sphereShape->getRadius() : -sphereShape->getRadius();
		timestep = (radius - distance) / denom;
		q = spherePos + timestep * sphereVel - radius * planeShape->getPlaneNormal();
		return 1;
	}
}

// This function has been overloaded to take a soft body and node index, instead of a rigid body like the one directly above
// The inner workings (math), however, is unchanged
int nPhysics::cPhysicsWorld::mIntersectMovingSpherePlane(iSoftBody* softBody, size_t nodeIndex, glm::vec3 movementVector, iRigidBody* plane, float& timestep, glm::vec3& q) const {

	// Get the plane shape
	nPhysics::iPlaneShape* planeShape = static_cast<nPhysics::iPlaneShape*>(plane->getShape());
	

	// Compute distance of node center to plane
	glm::vec3 nodePos;
	glm::vec3 planePos;
	float nodeRadius = 0.0f;
	softBody->getNodePosition(nodeIndex, nodePos);
	softBody->getNodeRadius(nodeIndex, nodeRadius);
	plane->getPosition(planePos);
	float distance = glm::dot(nodePos - planePos, planeShape->getPlaneNormal());
	if (glm::abs(distance) <= nodeRadius) {
		// The node is already overlapping the plane
		timestep = 0.0f;
		q = nodePos;
		return -1;
	}

	glm::vec3 nodeVel;
	softBody->getNodeVelocity(nodeIndex, nodeVel);
	float denom = glm::dot(planeShape->getPlaneNormal(), glm::normalize(nodeVel));
	if (denom * distance >= 0.0f) {
		// No intersection
		return 0;
	}
	else {
		// Sphere is moving towards the plane
		// Use +radius in computations if sphere in front of plane, else -radius
		float radius = distance > 0.0f ? nodeRadius : -nodeRadius;
		timestep = (radius - distance) / denom;
		q = nodePos + timestep * nodeVel - radius * planeShape->getPlaneNormal();
		return 1;
	}
}

int nPhysics::cPhysicsWorld::mIntersectMovingSphereSphere(iRigidBody* sphereA, iRigidBody* sphereB, float& accel) const {

	// Get sphere shapes
	iSphereShape* sphereShapeA = static_cast<iSphereShape*>(sphereA->getShape());
	iSphereShape* sphereShapeB = static_cast<iSphereShape*>(sphereB->getShape());

	// Get sphere positions
	glm::vec3 sphereAPos;
	glm::vec3 sphereBPos;
	sphereA->getPosition(sphereAPos);
	sphereB->getPosition(sphereBPos);

	// Get sphere velocities
	glm::vec3 sphereAVel;
	glm::vec3 sphereBVel;
	sphereA->getVelocity(sphereAVel);
	sphereB->getVelocity(sphereBVel);

	glm::vec3 s = sphereBPos - sphereAPos;
	glm::vec3 v = glm::normalize(sphereBVel - sphereAVel);
	float r = sphereShapeA->getRadius() + sphereShapeB->getRadius();

	return this->mSphereIntersectionTest(s, v, r, accel);
}

// This function handles the sphere distance check from intersectMovingSphereSphere
// It was moved so that it could be used for both sphere-sphere Rigid bodies and
// Rigid body-Soft body collisions
int nPhysics::cPhysicsWorld::mSphereIntersectionTest(glm::vec3 distance, glm::vec3 normalizedVelocity, float radiiSum, float & accel) const {

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

nPhysics::Derivative nPhysics::cPhysicsWorld::mEvaluate(const State& initialState, glm::vec3 t, float deltaTime, const Derivative& d) {

	State state;
	state.position = initialState.position + d.dx * deltaTime;
	state.velocity = initialState.velocity + d.dv * deltaTime;

	Derivative output;
	output.dx = state.velocity;
	output.dv += t;

	return output;
}

void nPhysics::cPhysicsWorld::mIntegrate(State& state, glm::vec3 t, float deltaTime) {

	Derivative a, b, c, d;

	a = this->mEvaluate(state, t, 0.0f, Derivative());
	b = this->mEvaluate(state, t, deltaTime * 0.5f, a);
	c = this->mEvaluate(state, t, deltaTime * 0.5f, b);
	d = this->mEvaluate(state, t, deltaTime, c);

	glm::vec3 dxdt = 1.0f / 6.0f *	(a.dx + 2.0f * (b.dx + c.dx) + d.dx);

	glm::vec3 dvdt = 1.0f / 6.0f * (a.dv + 2.0f * (b.dv + c.dv) + d.dv);

	state.position = state.position + dxdt * deltaTime;
	state.velocity = state.velocity + dvdt * deltaTime;

	return;
}

void nPhysics::cPhysicsWorld::mSpherePlaneCollisionResponse(sCollisionDetails& collision, float deltaTime) {

	// TODO: Make drop off value configurable
	float dropOff = 0.15f; // velocity decay

	float timeWithoutCollision = (deltaTime * collision.timeStep);
	float timeAfterCollision = deltaTime - timeWithoutCollision;

	// reflect velocity off the plane normal
	glm::vec3 reflectVec = glm::reflect(collision.contactVelocityA, collision.contactNormal) * (1.0f - dropOff);

	collision.bodyA->setVelocity(reflectVec);

	// Continue the time step
	glm::vec3 newPos;
	collision.bodyA->getPosition(newPos);

	glm::vec3 newVel;
	collision.bodyA->getVelocity(newVel);

	State state;
	state.position = newPos;
	state.velocity = newVel;
	this->mIntegrate(state, glm::vec3(0.0f), timeAfterCollision);
	collision.bodyA->setVelocity(state.velocity);
	collision.bodyA->setPosition(state.position);

	return;
}

void nPhysics::cPhysicsWorld::mSphereSphereCollisionResponse(sCollisionDetails& collision, float deltaTime) {

	float dropOff = 0.15f;

	float timeWithoutCollision = (deltaTime * collision.timeStep);
	float timeAfterCollision = deltaTime - timeWithoutCollision;

	glm::vec3 bodyACurPos;
	collision.bodyA->getPosition(bodyACurPos);

	glm::vec3 bodyBCurPos;
	collision.bodyB->getPosition(bodyBCurPos);

	glm::vec3 bodyAVel;
	collision.bodyA->getVelocity(bodyAVel);

	glm::vec3 bodyBVel;
	collision.bodyB->getVelocity(bodyBVel);

	// move the spheres back to where they collided
	glm::vec3 bodyAContact = bodyACurPos - (bodyAVel * timeAfterCollision);
	glm::vec3 bodyBContact = bodyBCurPos - (bodyBVel * timeAfterCollision);
	collision.bodyA->setPosition(bodyAContact);
	collision.bodyB->setPosition(bodyBContact);

	// Do inelastic collisions
	float bodyAMass;
	float bodyBMass;

	collision.bodyA->getMass(bodyAMass);
	collision.bodyB->getMass(bodyBMass);
	glm::vec3 bodyAInitVel = collision.contactVelocityA * (1.0f - dropOff);
	glm::vec3 bodyBInitVel = collision.contactVelocityB * (1.0f - dropOff);

	// Use formula found here to calculate new velocity: https://en.wikipedia.org/wiki/Inelastic_collision
	// TODO: Make  coefficient of restitution configurable
	float coefficient = 0.45f;
	glm::vec3 newVelA = ((((bodyAMass - bodyBMass) / (bodyAMass + bodyBMass)) * bodyAInitVel) + (((2 * bodyBMass) / (bodyAMass + bodyBMass)) * bodyBInitVel) * coefficient);
	glm::vec3 newVelB = ((((-bodyAMass + bodyBMass) / (bodyAMass + bodyBMass)) * bodyBInitVel) + (((2 * bodyAMass) / (bodyAMass + bodyBMass)) * bodyAInitVel) * coefficient);

	newVelA += 0.38f * collision.contactNormal * glm::dot(newVelB, collision.contactNormal);
	newVelB += 0.38f * collision.contactNormal * glm::dot(newVelA, collision.contactNormal);

	// Set the new velocities
	collision.bodyA->setVelocity(newVelA);
	collision.bodyB->setVelocity(newVelB);

	// Continue the time step
	glm::vec3 bodyAPos;
	glm::vec3 bodyBPos;

	collision.bodyA->getPosition(bodyAPos);
	collision.bodyB->getPosition(bodyBPos);

	State newStateA;
	newStateA.position = bodyAPos;
	newStateA.velocity = newVelA;

	this->mIntegrate(newStateA, glm::vec3(0.0f), timeAfterCollision);
	collision.bodyA->setVelocity(newStateA.velocity);
	collision.bodyA->setPosition(newStateA.position);

	State newStateB;
	newStateB.position = bodyBPos;
	newStateB.velocity = newVelB;

	this->mIntegrate(newStateB, glm::vec3(0.0f), timeAfterCollision);
	collision.bodyB->setVelocity(newStateB.velocity);
	collision.bodyB->setPosition(newStateB.position);

	return;
}

// Taken from page 79 of the Eriksen textbook
bool nPhysics::cPhysicsWorld::mAreAABBOverlaping(glm::vec3 boxAMin, glm::vec3 boxAMax, glm::vec3 boxBMin, glm::vec3 boxBMax) {

	// Exit with no intersection if separated along an axis
	if (boxAMax.x < boxBMin.x || boxAMin.x > boxBMax.x) return false;
	if (boxAMax.y < boxBMin.y || boxAMin.y > boxBMax.y) return false;
	if (boxAMax.z < boxBMin.z || boxAMin.z > boxBMax.z) return false;

	// Overlapping on all areas means AABBs are intersecting
	return true;
}

// Checks the AABB of two Collision bodies to see if they are in narrow phase range
bool nPhysics::cPhysicsWorld::mBroadPhaseTest(iCollisionBody* bodyA, iCollisionBody* bodyB) {

	glm::vec3 bodyAMin(0.0f);
	glm::vec3 bodyAMax(0.0f);
	glm::vec3 bodyBMin(0.0f);
	glm::vec3 bodyBMax(0.0f);

	bodyA->getAABB(bodyAMin, bodyAMax);
	bodyB->getAABB(bodyBMin, bodyBMax);

	return this->mAreAABBOverlaping(bodyAMin, bodyAMax, bodyBMin, bodyBMax);
}

/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#ifndef _nConvert_HG_
#define _nConvert_HG_

#include <gameMath.h>
#include "LinearMath/btVector3.h"

namespace nConvert {

	inline glm::vec3 toGlm(const btVector3& vecOut) {
		
		return glm::vec3(vecOut.x(), vecOut.y(), vecOut.z());
	}

	inline void toGlm(const btVector3& vecIn, glm::vec3& vecOut) {

		vecOut.x = vecIn.x();
		vecOut.y = vecIn.y();
		vecOut.z = vecIn.z();

		return;
	}

	inline btVector3 toBullet(const glm::vec3& vecOut) {

		return btVector3(vecOut.x, vecOut.y, vecOut.z);
	}

	inline void toBullet(const glm::vec3& vecIn, btVector3& vecOut) {

		vecOut.setValue(vecIn.x, vecIn.y, vecIn.z);

		return;
	}

	inline float toGlm(const btScalar& scalarIn) {

		return static_cast<float>(scalarIn);
	}

	inline void toGlm(const btScalar& scalarIn, float& valueOut) {

		valueOut = static_cast<float>(scalarIn);

		return;
	}

	inline btScalar toBullet(const float& floatIn) {

		return btScalar(floatIn);
	}

	inline void toBullet(const float& floatIn, btScalar& scalarOut) {

		scalarOut = btScalar(floatIn);

		return;
	}

	inline glm::quat toGlm(const btQuaternion& orientationIn) {

		return glm::quat(
			toGlm(orientationIn.w()),
			toGlm(orientationIn.x()),
			toGlm(orientationIn.y()),
			toGlm(orientationIn.z()));
	}

	inline void toGlm(const btQuaternion& orientationIn, glm::quat& orientationOut) {

		orientationOut.x = toGlm(orientationIn.x());
		orientationOut.y = toGlm(orientationIn.y());
		orientationOut.z = toGlm(orientationIn.z());
		orientationOut.w = toGlm(orientationIn.w());

		return;
	}

	inline btQuaternion toBullet(const glm::quat& orientationIn) {

		return btQuaternion(toBullet(orientationIn.x),
							toBullet(orientationIn.y),
							toBullet(orientationIn.z),
							toBullet(orientationIn.w));
	}

	inline void toBullet(const glm::quat orientationIn, btQuaternion& orientationOut) {

		orientationOut.setX(orientationIn.x);
		orientationOut.setY(orientationIn.y);
		orientationOut.setZ(orientationIn.z);
		orientationOut.setW(orientationIn.w);

		return;
	}

	inline glm::quat quaternionFromEuler(const glm::vec3& eulerAngles) {

		glm::vec3 orientation = glm::vec3(glm::radians(eulerAngles.x), glm::radians(eulerAngles.y), glm::radians(eulerAngles.z));
		return glm::quat(glm::vec3(orientation.x, orientation.y, orientation.z));
	}
}

#endif // !_nConvert_HG_


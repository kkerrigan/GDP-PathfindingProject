#ifndef _Global_HG_
#define _Global_HG_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>

template <class T>
T getRandInRange(T min, T max) {

	double value =
		min + static_cast <float> (rand())
		/ (static_cast <float> (RAND_MAX / (static_cast<float>(max - min))));

	return static_cast<T>(value);
}

#endif

/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#ifndef _cInstancedMeshComponent_HG_
#define _cInstancedMeshComponent_HG_

#include <gameMath.h>

#include "cComponent.h"

class cInstancedMeshComponent : public cComponent {
public:
	cInstancedMeshComponent();
	virtual ~cInstancedMeshComponent();

	glm::mat4* modelMatrices;
	glm::vec4 color;

	unsigned int getAmount();
	void setAmount(unsigned int amount);

private:
	unsigned int mAmount;
};


#endif // !_cInstancedMeshComponent_HG_


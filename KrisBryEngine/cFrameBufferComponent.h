/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#ifndef _cFrameBufferComponent_HG_
#define _cFrameBufferComponent_HG_

#include "cComponent.h"
#include "cFBO.h"

class cFrameBufferComponent : public cComponent {
public:
	cFrameBufferComponent();
	virtual ~cFrameBufferComponent();

	cFBO* pFbo;

	void inititFrameBuffer(int texNumber);
	int getFboTextureUnit() const;

private:
	int mFboTextureUnit;
};


#endif // !_cFrameBufferComponent_HG_


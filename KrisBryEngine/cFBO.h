#ifndef _cFBO_HG_
#define _cFBO_HG_

#include <string>

#include "Global.h"

class cFBO {
public:
	cFBO();

	GLuint ID;						
	GLuint colourTexture_0_ID;
	GLuint normalTexture_1_ID;
	GLuint vertexWorldPos_2_ID;

	//GLuint TBDTexture_2_ID;
	//GLuint TBDTexture_3_ID;

	GLuint depthTexture_ID;
	GLint width;		// = 512 the WIDTH of the framebuffer, in pixels;
	GLint height;

	// Inits the FBP
	bool init(int width, int height, std::string &error);
	bool shutdown(void);
	
	// Calls shutdown(), then init()
	bool reset(int width, int height, std::string &error);
	
	void clearBuffers(bool bClearColour = true, bool bClearDepth = true);

	void clearColourBuffer(int bufferindex);
	void clearAllColourBuffers(void);
	void clearDepthBuffer(void);
	void clearStencilBuffer( int clearColour, int mask = 0xFF );

	int getMaxColourAttachments(void);
	int getMaxDrawBuffers(void);
};

#endif

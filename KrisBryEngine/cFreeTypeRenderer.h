#ifndef _cFreeTypeRenderer_HG_
#define _cFreeTypeRenderer_HG_


#include <gameMath.h>
#include "Global.h"

#include <ft2build.h>
#include FT_FREETYPE_H 

class cFreeTypeRenderer {
public:
	// Singleton setup
	static cFreeTypeRenderer* getInstance();
	cFreeTypeRenderer(cFreeTypeRenderer const&) = delete;
	void operator=(cFreeTypeRenderer const&) = delete;

	FT_Library ft;
	FT_Face face;
	
	struct point {
		GLfloat x;
		GLfloat y;
		GLfloat s;
		GLfloat t;
	};
	
	

	// Text rendering values
	float sx;
	float sy;
	GLfloat yoffset;
	GLfloat xoffset;

	float offset_step;

	bool initText(int fontType);
	void renderText(const char* text, float x, float y, float sx, float sy, glm::vec4 color);

private:
	cFreeTypeRenderer();
	~cFreeTypeRenderer();

	unsigned int dp_vbo;
};

#endif

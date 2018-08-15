#pragma once
#include <GL/glew.h>

namespace fb {

	

	class Quad {
	public:
		Quad();
		GLuint vao;
	};

	Quad getQuad();
	void initStaticGeometry();
}

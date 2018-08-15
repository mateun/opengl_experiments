#pragma once
#include <GL/glew.h>

namespace fb {
	class ShaderProgram {
	public:
		ShaderProgram() {}
		ShaderProgram(const char* vsFileName, const char* fsFileName);
		GLuint _progHandle;
	};
}


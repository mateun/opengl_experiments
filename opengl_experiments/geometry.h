#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "shaderprogram.h"

namespace fb {

	
	class XAxis {
		public:
			XAxis();
			void render(glm::mat4 translationMatrix, glm::mat4 viewMatrix, glm::mat4 projMatrix, glm::mat4 rotMatrix,
				glm::mat4 scaleMatrix, ShaderProgram shader);
			GLuint vao;


	};

	class Quad {
	public:
		Quad();
		GLuint vao;
	};

	XAxis getXAxis();
	Quad getQuad();
	void initStaticGeometry();
}

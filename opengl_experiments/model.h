#pragma once
#include <inttypes.h>
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "shaderprogram.h"
#include "texture.h"


namespace fb {

	class Model {
	public:
		Model(float* pos, int posLen, float* normals, int normalsLen, float* uvs, int uvsLen);
		void render(glm::mat4 translationMatrix, glm::mat4 viewMatrix, glm::mat4 projMatrix, glm::mat4 rotMatrix, 
				glm::mat4 scaleMatrix, ShaderProgram shader,
				Texture texture, glm::vec3 lightPos, Texture* shadowMap);

		float* _positions;
		float* _normals;
		float* _uvs;
		int _posLen;
		int _normalsLen;
		int _uvsLen;

		uint32_t* indeces;
		GLuint _vbPos;
		GLuint _vbCol;
		GLuint _vbNorm;
		GLuint _vbTex;
		GLuint _vao;
		GLuint _ib;

	};

}



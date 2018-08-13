#pragma once
#include <inttypes.h>
#include <GL/glew.h>

namespace fb {

	class Model {
	public:
		Model(float* pos, int posLen, float* normals, int normalsLen, float* uvs, int uvsLen);

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



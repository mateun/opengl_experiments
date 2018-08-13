#include "stdafx.h"
#include "model.h"

fb::Model::Model(float* pos, int posLen, float* normals, int normalsLen, float* uvs, int uvsLen) {
	_positions = pos;
	_normals = normals;
	_uvs = uvs;
	_posLen = posLen;
	_normalsLen = normalsLen;
	_uvsLen = uvsLen;

	// Setup all the OpenGL stuff
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	glGenBuffers(1, &_vbPos);
	glBindBuffer(GL_ARRAY_BUFFER, _vbPos);
	glBufferData(GL_ARRAY_BUFFER, _posLen * 3 * 4, _positions, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}
#include "stdafx.h"
#include "geometry.h"

static fb::Quad* quad;

void fb::initStaticGeometry() {
	quad = new Quad();
}

fb::Quad fb::getQuad() {
	return *quad;
}

fb::Quad::Quad()
{
	GLfloat quadData[18] = {
		-0.5, -0.5, -0.5,
		0.5, -0.5, -0.5,
		0.5, 0.5, -0.5,
		-0.5, -0.5, -0.5,
		0.5, 0.5, -0.5,
		-0.5, 0.5, -0.5,
	};

	GLfloat quadUVs[12] = {
		0, 1,
		1, 1,
		1, 0,
		0, 1,
		1, 0,
		0, 0
	};

	GLuint vbo;
	GLuint vbuvs;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * 6, quadData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &vbuvs);
	glBindBuffer(GL_ARRAY_BUFFER, vbuvs);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * 6, quadUVs, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
}


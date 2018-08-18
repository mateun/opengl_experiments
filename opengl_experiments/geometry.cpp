#include "stdafx.h"
#include "geometry.h"
#include <glm/gtc/type_ptr.hpp>

static fb::Quad* quad;
static fb::XAxis* xAxis;

void fb::initStaticGeometry() {
	quad = new Quad();
	xAxis = new XAxis();

}

fb::XAxis fb::getXAxis() {
	return *xAxis;
}

fb::Quad fb::getQuad() {
	return *quad;
}

fb::XAxis::XAxis() {
	GLfloat xAxisData[6] = {
		0, 0, 0,
		0, 0.5, 0
	};

	GLuint vbo;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * 2, xAxisData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}

void fb::XAxis::render(glm::mat4 translationMatrix, glm::mat4 viewMatrix, glm::mat4 projMatrix, glm::mat4 rotMatrix, glm::mat4 scaleMatrix, ShaderProgram shader)
{
	glUseProgram(shader._progHandle);

	glm::mat4 mwv = glm::mat4(1.0f);
	mwv = viewMatrix * translationMatrix * rotMatrix * scaleMatrix;

	GLuint mwvId = glGetUniformLocation(shader._progHandle, "mwv");
	GLuint mpId = glGetUniformLocation(shader._progHandle, "mp");

	glUniformMatrix4fv(mwvId, 1, false, glm::value_ptr(mwv));
	glUniformMatrix4fv(mpId, 1, false, glm::value_ptr(projMatrix));


	glBindVertexArray(vao);
	glDrawArrays(GL_LINE, 0, 6);
	glBindVertexArray(0);
	
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


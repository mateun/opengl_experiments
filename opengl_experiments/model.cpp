#include "stdafx.h"
#include "model.h"
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &_vbTex);
	glBindBuffer(GL_ARRAY_BUFFER, _vbTex);
	glBufferData(GL_ARRAY_BUFFER, _uvsLen * 2 * 4, _uvs, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, 0, 0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &_vbNorm);
	glBindBuffer(GL_ARRAY_BUFFER, _vbNorm);
	glBufferData(GL_ARRAY_BUFFER, _normalsLen * 3 * 4, _normals, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void fb::Model::render(glm::mat4 translationMatrix, glm::mat4 viewMatrix, glm::mat4 projMatrix, glm::mat4 rotMatrix, 
	glm::mat4 scaleMatrix, ShaderProgram shader, Texture texture, glm::vec3 lightPos, Texture* shadowMap)
{
	glUseProgram(shader._progHandle);

	glm::mat4 mwv = glm::mat4(1.0f);
	mwv = viewMatrix * translationMatrix * rotMatrix * scaleMatrix;

	GLuint mwvId = glGetUniformLocation(shader._progHandle, "mwv");
	GLuint mpId = glGetUniformLocation(shader._progHandle, "mp");

	glUniformMatrix4fv(mwvId, 1, false, glm::value_ptr(mwv));
	glUniformMatrix4fv(mpId, 1, false, glm::value_ptr(projMatrix));
	glUniform3fv(0, 1, glm::value_ptr(lightPos));

	// Provide the light matrix
	if (shadowMap) {
		fb::Camera lightCam(lightPos, glm::vec3(0, 0, 0));
		lightCam.initializeOrthographic(-70, 70, -70, 70);
		
		glm::mat4 lightMV = lightCam.view * translationMatrix * rotMatrix * scaleMatrix;
		GLuint lightViewId = glGetUniformLocation(shader._progHandle, "lightView");
		GLuint lightProjId = glGetUniformLocation(shader._progHandle, "lightProj");
		glUniformMatrix4fv(lightViewId, 1, false, glm::value_ptr(lightMV));
		glUniformMatrix4fv(lightProjId, 1, false, glm::value_ptr(lightCam.proj));
	}

	texture.bind(0);
	if (shadowMap) {
		shadowMap->bind(1);
	}
	glBindVertexArray(_vao);
	glDrawArrays(GL_TRIANGLES, 0, _posLen);
	glBindVertexArray(0);
	texture.unbind();
	if (shadowMap) shadowMap->unbind();

}

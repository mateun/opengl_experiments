#include "stdafx.h"
#include "sprite.h"
#include "geometry.h"
#include "shaderprogram.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

fb::Sprite::Sprite(Texture texture, ShaderManager shaderManager, int screenWidth, int screenHeight) : _tex(texture), _shaderManager(shaderManager),
	_screenWidth(screenWidth), _screenHeight(screenHeight)
{
}

void fb::Sprite::renderFlippedDepth(int screenX, int screenY, int depth)
{

	glDisable(GL_CULL_FACE);

	Quad q = fb::getQuad();
	fb::ShaderProgram shader2d = _shaderManager.getShader("depthShader");

	glm::mat4 orthomp = glm::ortho(0.0, (double)_screenWidth, (double)_screenHeight, 0.0, 0.1, 100.0);
	// The "-1" scale here is necessary due to our top-down ortho matrix. If you don't do this,
	// the triangle winding will be culled.
	glm::mat4 orthoscale = glm::scale(glm::mat4(1.0f), glm::vec3(_tex.w * _scaleX, _tex.h *_scaleY * 1, 1));
	glm::mat4 orthotransl = glm::translate(glm::mat4(1.0f), glm::vec3(screenX, screenY, depth));
	glm::mat4 orthov = glm::lookAtRH(glm::vec3(0, 0, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	GLuint mpIndex = glGetUniformLocation(shader2d._progHandle, "mp");
	GLuint mwvIndex = glGetUniformLocation(shader2d._progHandle, "mwv");

	glUseProgram(shader2d._progHandle);
	_tex.bind();
	glUniformMatrix4fv(mwvIndex, 1, false, glm::value_ptr(orthov * orthotransl * orthoscale));
	glUniformMatrix4fv(mpIndex, 1, false, glm::value_ptr(orthomp));
	glBindVertexArray(fb::getQuad().vao);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	_tex.unbind();

	glEnable(GL_CULL_FACE);

}



void fb::Sprite::renderFlipped(int screenX, int screenY, int depth)
{

	glDisable(GL_CULL_FACE);

	Quad q = fb::getQuad();
	fb::ShaderProgram shader2d = _shaderManager.getShader("spriteShader");

	glm::mat4 orthomp = glm::ortho(0.0, (double)_screenWidth, (double)_screenHeight,  0.0, 0.1, 100.0);
	// The "-1" scale here is necessary due to our top-down ortho matrix. If you don't do this,
	// the triangle winding will be culled.
	glm::mat4 orthoscale = glm::scale(glm::mat4(1.0f), glm::vec3(_tex.w * _scaleX, _tex.h *_scaleY * 1, 1));
	glm::mat4 orthotransl = glm::translate(glm::mat4(1.0f), glm::vec3(screenX, screenY, depth));
	glm::mat4 orthov = glm::lookAtRH(glm::vec3(0, 0, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	GLuint mpIndex = glGetUniformLocation(shader2d._progHandle, "mp");
	GLuint mwvIndex = glGetUniformLocation(shader2d._progHandle, "mwv");

	glUseProgram(shader2d._progHandle);
	_tex.bind();
	glUniformMatrix4fv(mwvIndex, 1, false, glm::value_ptr(orthov * orthotransl * orthoscale));
	glUniformMatrix4fv(mpIndex, 1, false, glm::value_ptr(orthomp));
	glBindVertexArray(fb::getQuad().vao);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	_tex.unbind();

	glEnable(GL_CULL_FACE);

}

void fb::Sprite::render(int screenX, int screenY, int depth)
{
	Quad q = fb::getQuad();
	fb::ShaderProgram shader2d = _shaderManager.getShader ("spriteShader");

	glm::mat4 orthomp = glm::ortho(0.0, (double)_screenWidth, (double) _screenHeight, 0.0, 0.1, 100.0);
	// The "-1" scale here is necessary due to our top-down ortho matrix. If you don't do this,
	// the triangle winding will be culled.
	glm::mat4 orthoscale = glm::scale(glm::mat4(1.0f), glm::vec3(_tex.w * _scaleX, _tex.h *_scaleY * -1, 1));
	glm::mat4 orthotransl = glm::translate(glm::mat4(1.0f), glm::vec3(screenX, screenY, depth));
	glm::mat4 orthov = glm::lookAtRH(glm::vec3(0, 0, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	GLuint mpIndex = glGetUniformLocation(shader2d._progHandle, "mp");
	GLuint mwvIndex = glGetUniformLocation(shader2d._progHandle, "mwv");

	glUseProgram(shader2d._progHandle);
	_tex.bind();
	glUniformMatrix4fv(mwvIndex, 1, false, glm::value_ptr(orthov * orthotransl * orthoscale));
	glUniformMatrix4fv(mpIndex, 1, false, glm::value_ptr(orthomp));
	glBindVertexArray(fb::getQuad().vao);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	_tex.unbind();

}

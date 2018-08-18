#include "stdafx.h"
#include "framebuffer.h"

fb::FrameBuffer::FrameBuffer(int width, int height)
{
	glGenFramebuffers(1, &_fbHandle);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbHandle);
	GLuint colTex;
	glGenTextures(1, &colTex);
	glBindTexture(GL_TEXTURE_2D, colTex);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLuint depthTex;
	glGenTextures(1, &depthTex);
	glBindTexture(GL_TEXTURE_2D, depthTex);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT24, width, height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colTex, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTex, 0);

	GLint result;
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE, &result);
	if (result == 0) {
		GLenum error = glGetError();
		SDL_Log("Error in FBO! %d", error);
	}

	GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0 };
	//glDrawBuffers(1, draw_buffers);

	_colorTexture._textureHandle = colTex;
	_colorTexture.w = width;
	_colorTexture.h = height;
	_depthTexture._textureHandle = depthTex;
	_depthTexture.w = width;
	_depthTexture.h = height;

	GLenum err = glGetError();
	if (err != 0) SDL_Log("error at fb: %d", err);

	GLenum completeness = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (completeness != GL_FRAMEBUFFER_COMPLETE) {
		SDL_Log("error with FBO completeness!");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void fb::FrameBuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _fbHandle);
}

void fb::FrameBuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

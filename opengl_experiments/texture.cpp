#include "stdafx.h"
#include "texture.h"

fb::Texture::Texture(const char * imageFileName, bool transparentBitmap)
{
	
	glGenTextures(1, &_textureHandle);
	glBindTexture(GL_TEXTURE_2D, _textureHandle);
	

	SDL_Surface* surface = SDL_LoadBMP(imageFileName);
	w = surface->w;
	h = surface->h;
	if (transparentBitmap) 
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_BGR, GL_UNSIGNED_BYTE, surface->pixels);
	else 
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, surface->w, surface->h, 0, GL_BGR, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	SDL_FreeSurface(surface);
}

void fb::Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, _textureHandle);
}

void fb::Texture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

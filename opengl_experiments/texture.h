#pragma once
#include <GL/glew.h>
#include <SDL.h>


namespace fb {
	class Texture {
	public: 
		Texture() {}
		Texture(const char* imageFileName);
		void bind();
		void unbind();
		GLuint _textureHandle;
	
	};
}



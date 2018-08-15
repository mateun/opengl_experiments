#pragma once
#include <GL/glew.h>
#include <SDL.h>


namespace fb {
	class Texture {
	public: 
		Texture() {}
		Texture(const char* imageFileName, bool transparentBitmap);
		void bind();
		void unbind();
		GLuint _textureHandle;
	
	};
}



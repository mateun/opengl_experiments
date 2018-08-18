#pragma once
#include <GL/glew.h>
#include <SDL.h>


namespace fb {
	class Texture {
	public: 
		Texture() {}
		Texture(const char* imageFileName, bool transparentBitmap);
		void bind(GLuint unit);
		void unbind();
		GLuint _textureHandle;
		int w;
		int h;
	
	};
}



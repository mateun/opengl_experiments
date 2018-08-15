#pragma once
#include <GL/glew.h>
#include "texture.h"

namespace fb {
	class FrameBuffer {
	public:
		FrameBuffer(int width, int height);
		void bind();
		void unbind();

		GLuint _fbHandle;
		Texture _colorTexture;
		Texture _depthTexture;
	};
}

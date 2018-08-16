#pragma once
#include "texture.h"
#include "shaderprogram.h"

namespace fb {
	class Sprite {
	public:
		Sprite(Texture texture, ShaderManager shaderManager, int screenWidth, int screenHeight);
		void render(int screenX, int screenY, int depth);
		void renderFlipped(int screenX, int screenY, int depth);
		void scale(float sx, float sy) {
			_scaleX = sx;
			_scaleY = sy;
		}
	private:
		Texture _tex;
		ShaderManager _shaderManager;
		int _screenWidth;
		int _screenHeight;
		float _scaleX = 1;
		float _scaleY = 1;
	};

}

#pragma once
#include "texture.h"

namespace fb {
	class Sprite {
	public:
		Sprite(Texture texture);
		void render(int screenX, int screenY, int depth);
	private:
		Texture _tex;
	};

}

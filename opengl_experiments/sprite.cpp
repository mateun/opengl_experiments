#include "stdafx.h"
#include "sprite.h"
#include "geometry.h"

fb::Sprite::Sprite(Texture texture) : _tex(texture)
{
}

void fb::Sprite::render(int screenX, int screenY, int depth)
{
	Quad q = fb::getQuad();

}

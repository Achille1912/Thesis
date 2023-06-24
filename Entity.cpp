#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include "Entity.h"

Entity::Entity(float p_x, float p_y, SDL_Texture *p_tex)
{
	x = p_x;
	y = p_y;
	tex = p_tex;
	currentFrame.x = 0;
	currentFrame.y = 0;
	currentFrame.w = 32;
	currentFrame.h = 32;
}

#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>



class Entity {
public:
	Entity(float p_x, float p_y, SDL_Texture* p_tex);

	float getX() { return x; }

	float getY() { return y; }

	void setY(float p_y) { y = p_y; }

	SDL_Texture* getTex() { return tex; }

	SDL_Rect getCurrentFrame() { return currentFrame; }

private:
	float x, y;
	SDL_Rect currentFrame;
	SDL_Texture* tex;

};
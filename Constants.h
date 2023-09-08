#pragma once

#include <SDL_render.h>

const int PIXELS_PER_METER = 50;

const float PI = 3.14f;

const float SmallAmount = 0.0005f;

const float NotSoSmallAmount = 0.3f;

const int SCREEN_WIDTH = 800;

const int SCREEN_HEIGHT = 600;

const int ANGULAR_FACTOR = 38;

const SDL_Color default_color = { 0, 255, 0, 255 };

enum Levels
{
	BASIC,
	ANGRY_BIRDS,
	PEGGLE,
	STAIRS
};

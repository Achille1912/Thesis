#pragma once

#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "Game.h"

class TextureManager {
public:
	static SDL_Texture* LoadTexture(const char* t_fileName, SDL_Renderer* t_renderer);
};


#endif
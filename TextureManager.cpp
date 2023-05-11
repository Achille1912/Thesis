#include "TextureManager.h"

SDL_Texture* TextureManager::LoadTexture(const char* t_fileName, SDL_Renderer* t_renderer) {
	SDL_Surface* tmpSurface = IMG_Load(t_fileName);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(t_renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);

	return tex;
}
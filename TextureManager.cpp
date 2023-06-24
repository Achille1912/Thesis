#include "TextureManager.h"
/**
 * @brief Load a new Texture
 *
 * @param t_fileName
 * @param t_renderer
 * @return SDL_Texture*
 */
SDL_Texture *TextureManager::LoadTexture(const char *t_fileName, SDL_Renderer *t_renderer)
{
	SDL_Surface *tmpSurface = IMG_Load(t_fileName);
	SDL_Texture *tex = SDL_CreateTextureFromSurface(t_renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);

	return tex;
}
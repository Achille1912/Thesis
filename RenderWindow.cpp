#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "RenderWindow.h"
#include "Entity.h"

RenderWindow::RenderWindow(const char *p_title, int p_w, int p_h)
	: window(NULL), renderer(NULL)
{
	window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		std::cout << "Errore" << SDL_GetError() << std::endl;
	}

	// SDL_RENDERER_ACCELERATED means "Go faster as you can"
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

SDL_Texture *RenderWindow::loadTexture(const char *p_filePath)
{
	SDL_Texture *texture = NULL;
	texture = IMG_LoadTexture(renderer, p_filePath);
	if (texture == NULL)
	{
		std::cout << "Failed load" << SDL_GetError() << std::endl;
	}
	return texture;
}

void RenderWindow::cleanUp()
{
	SDL_DestroyWindow(window);
}

void RenderWindow::clear()
{
	SDL_RenderClear(renderer);
}

void RenderWindow::render(Entity &p_ent)
{

	SDL_Rect src;
	src.x = p_ent.getCurrentFrame().x;
	src.y = p_ent.getCurrentFrame().y;
	src.w = p_ent.getCurrentFrame().w;
	src.h = p_ent.getCurrentFrame().h;

	SDL_Rect dst;
	dst.x = p_ent.getX();
	dst.y = p_ent.getY();
	dst.w = p_ent.getCurrentFrame().w;
	dst.h = p_ent.getCurrentFrame().h;

	SDL_RenderCopy(renderer, p_ent.getTex(), &src, &dst);
}

void RenderWindow::display()
{
	SDL_RenderPresent(renderer);
}
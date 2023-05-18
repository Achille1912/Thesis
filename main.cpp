#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

#include "Game.h"
#include "Utils.h"
#include "Constants.h"

Game* game = Game::instance();

int main(int argc, char** argv) {

	Uint32 lastTime = SDL_GetTicks();
	Uint32 currentTime = lastTime;
	float deltaTime = 0;

	game->init("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, false);

	while (game->running()) {
		// Calculate delta time
		currentTime = SDL_GetTicks();
		deltaTime = (float)(currentTime - lastTime) / 1000;
		lastTime = currentTime;

		game->handleEvents();

		game->update(deltaTime);

		game->render();
	}
	return 0;
}


#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

#include "Game.h"
#include "Utils.h"
#include "Constants.h"
#include "timeUtils.h"

Game* game = Game::instance();

int main(int argc, char** argv)
{

	game->Init("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, false);

	agp::FPS fps;
	agp::Timer<float> frameTimer;

	float dt = 0.01f; // physics integration
	float accumulator = 0;

	while (game->Running())
	{
		// Calculate delta time
		game->HandleEvents();
		float frameTime = frameTimer.restart();

		accumulator += frameTime;
		while (accumulator >= dt)
		{
			game->Update(dt);
			accumulator -= dt;
		}

		game->Render();
		fps.update();
	}

	return 0;
}
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <string>

#include "Game.h"
#include "Graphics.h"
#include "TextureManager.h"
#include "Vector2.h"
#include "DynamicBody.h"
#include "Collision.h"
#include "StaticBody.h"
#include "Constants.h"
#include "Utils.h"

DynamicBody* player;

// Singleton
Game* Game::m_uniqueInstance = 0;
Game* Game::instance()
{
	if (m_uniqueInstance == 0)
		m_uniqueInstance = new Game();
	return m_uniqueInstance;
}

void Game::init(const char* t_title, int t_xpos, int t_ypos, int t_width, int t_height, bool t_fullscreen) {
	// INITIAL STUFF
	m_isRunning = Graphics::OpenWindow(t_title, t_xpos, t_ypos, t_width, t_height, t_fullscreen);

	m_world = new World(-9.8, false);

	// OBJECTS INITIALIZATION

	int mass1 = 1;
	int mass2 = 10;
	
	m_world->AddGameObject(new DynamicBody("./gfx/woodBox.jpg", Graphics::renderer, 30, 10, Vector2f(0,0) , 46, 46, mass2, 0.4));
	m_world->AddGameObject(new DynamicBody("./gfx/woodBox.jpg", Graphics::renderer, 100, 100, Vector2f(0, 0), 46, 46, mass2, 0.4));

	m_world->AddGameObject(new StaticBody("./gfx/ground.png", Graphics::renderer, 10, 300, 300, 48, mass2, 20, 15, 0.4));

	m_world->AddGameObject(new StaticBody("./gfx/ground.png", Graphics::renderer, 300, 450, 300, 48, mass2, 20, -15, 0.4));

	m_world->AddGameObject(new StaticBody("./gfx/ground.png", Graphics::renderer, 10, 550, 600, 48, mass2, 20, 0, 0.4));

	player = dynamic_cast<DynamicBody*>(m_world->GetGameObjects()[0]);
}

void Game::handleEvents() {
	Vector2f dir(0,0);
	float step = 8;
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type){
	case SDL_QUIT:
		m_isRunning = false;
		break;
	case SDL_MOUSEWHEEL:
		if (event.wheel.y > 0) {
			Graphics::scale *= 1.1f;
		}
		else if (event.wheel.y < 0) {
			Graphics::scale /= 1.1f;
		}

		break;
	case SDL_MOUSEBUTTONDOWN:
		if (event.button.button == SDL_BUTTON_LEFT)
		{
			int x, y;
			Uint32 mouseState = SDL_GetMouseState(&x, &y);

			m_world->AddGameObject(new DynamicBody("./gfx/woodBox.jpg", Graphics::renderer, x,y, Vector2f(0, 0), 46, 46, (1 + (rand() % 10)), 0.4));

		}
		break;
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym) {
			
		case SDLK_SPACE:
			m_world->AddGameObject(new DynamicBody("./gfx/grass.png", Graphics::renderer, (1 + (rand() % 600)), (1 + (rand() % 600)), Vector2f(0, 0), (10 + (rand() % 60)), (1 + (rand() % 50)), (1 + (rand() % 10)), 0.8));
			break;

		case SDLK_f:
			if(!Graphics::debug)
				Graphics::debug = true;
			else
				Graphics::debug = false;
			break;

		case SDLK_UP:
			player->move(Vector2f(0, -step*2));
			break;

		case SDLK_DOWN:
			player->move(Vector2f(0, step));
			break;

		case SDLK_LEFT:
			player->move(Vector2f(-step, 0));
			break;

		case SDLK_RIGHT:
			player->move(Vector2f(step, 0));
			
			break;
		
		case SDLK_r:
			
			player->rotate();
			std::cout << player->getRotation() << std::endl;
			break;
		}

		break;
	default:
		break;
	}
	player->move(dir);
}

void Game::update(float dt) {
	m_world->Update(dt, 30);
}

void Game::render(){
	Graphics::ClearScreen();
	SDL_Rect destrect = { 0, 0, 3584, 480 };
	SDL_RenderCopy(Graphics::renderer, Graphics::sfondo_texture, NULL, &destrect);

	for (int i = 0; i < m_world->GetGameObjects().size(); i++) {
		m_world->GetGameObjects()[i]->render();
		
		SDL_SetTextureColorMod(m_world->GetGameObjects()[i]->getTex(), 255, 255, 255);
	}

	Graphics::RenderScale();
	Graphics::RenderFrame();

}

void Game::clean(){
	Graphics::CloseWindow();
}



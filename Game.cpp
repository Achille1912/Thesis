#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <SDL_ttf.h>

#include "Game.h"
#include "Graphics.h"
#include "TextureManager.h"
#include "Vector2.h"
#include "DynamicBody.h"
#include "Collision.h"
#include "StaticBody.h"
#include "Constants.h"
#include "Utils.h"

DynamicBody *player;
bool level0 = true;

// Singleton
Game *Game::m_uniqueInstance = 0;
Game *Game::instance()
{
	if (m_uniqueInstance == 0)
		m_uniqueInstance = new Game();
	return m_uniqueInstance;
}

/**
 * @brief Create Game Object
 *
 * @param t_title
 * @param t_xpos
 * @param t_ypos
 * @param t_width
 * @param t_height
 * @param t_fullscreen
 */
Levels level_name = STAIRS;
void Game::Init(const char *t_title, int t_xpos, int t_ypos, int t_width, int t_height, bool t_fullscreen)
{
	// INITIAL STUFF
	m_isRunning = Graphics::OpenWindow(t_title, t_xpos, t_ypos, t_width, t_height, t_fullscreen);
	m_world = new World(-9.8f, false);
	m_level_builder = new LevelBuilder(m_world);
	// OBJECTS INITIALIZATION
	float mass1 = 1;
	float mass2 = 100;


	switch (level_name)
	{
	case BASIC:
		player = m_level_builder->LoadLevel(Levels::BASIC);
		break;
	case ANGRY_BIRDS:
		player = m_level_builder->LoadLevel(Levels::ANGRY_BIRDS);
		break;
	case PEGGLE:
		player = m_level_builder->LoadLevel(Levels::PEGGLE);
		break;
	case STAIRS:
		player = m_level_builder->LoadLevel(Levels::STAIRS);
		break;
	default:
		break;
	}

}

/**
 * @brief Handle Game Events
 *
 */
void Game::HandleEvents()
{
	Vector2f dir(0, 0);
	float step = 8;
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_QUIT:
		m_isRunning = false;
		break;
	case SDL_MOUSEWHEEL:
		if (event.wheel.y > 0)
			Graphics::scale *= 1.1f;

		else if (event.wheel.y < 0)
			Graphics::scale /= 1.1f;

		break;
	case SDL_MOUSEBUTTONDOWN:
		if (event.button.button == SDL_BUTTON_LEFT)
		{
			int x, y;
			Uint32 mouseState = SDL_GetMouseState(&x, &y);
			if (level0)
			{
				m_world->AddGameObject(new DynamicBody("./gfx/woodBox.jpg", Graphics::renderer, x, y, Vector2f(0, 0), 46, 46, (1 + (rand() % 10)), 0.4f));
			}
			else
			{
				Vector2f vel = Vector2f(x, y) - player->GetCenter();
				player->AddVel(vel);
			}
		}
		break;
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym)
		{
		case SDLK_UP:
			player->Move(Vector2f(0, -step * 2));
			break;

		case SDLK_DOWN:
			player->Move(Vector2f(0, step));
			break;

		case SDLK_LEFT:
			player->Move(Vector2f(-step, 0));
			break;

		case SDLK_RIGHT:
			player->Move(Vector2f(step, 0));

			break;

		case SDLK_r:

			player->Rotate();
			std::cout << player->GetRotation() << std::endl;
			break;
		case SDLK_m:
			if (m_world->m_basic)
			{
				m_world->m_basic = false;
				Utils::Print("Mode: Rotation");
			}
			else
			{
				m_world->m_basic = true;
				Utils::Print("Mode: Basic");
			}
			break;
		}

		break;
	default:
		break;
	}
	player->Move(dir);
}

/**
 * @brief Update World
 *
 * @param dt
 */
void Game::Update(float dt)
{
	m_world->Update(dt, 80);
}

/**
 * @brief Render Graphics
 *
 */
void Game::Render()
{
	Graphics::ClearScreen();

	/*SDL_Rect* src_rect = new SDL_Rect();
	src_rect->x = 0;
	src_rect->y = 0;
	src_rect->w = 400;
	src_rect->h = 230;

	if (level_name != BASIC)
		SDL_RenderCopy(Graphics::renderer, Graphics::background_texture, src_rect, NULL);*/

	for (int i = 0; i < m_world->GetGameObjects().size(); i++)
	{
		m_world->GetGameObjects()[i]->Render();
	}

	//
	TTF_Font *font = TTF_OpenFont("./Roboto-Black.ttf", 14);
	SDL_Color color = {0, 0, 0};

	std::string text = "Number of objects: " + std::to_string(m_world->GetGameObjects().size());
	SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), color);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(Graphics::renderer, surface);

	SDL_FreeSurface(surface);
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = 200;
	rect.h = 50;

	SDL_RenderCopy(Graphics::renderer, texture, NULL, &rect);
	//
	SDL_RenderPresent(Graphics::renderer);

	Graphics::RenderScale();
	Graphics::RenderFrame();
}

/**
 * @brief Close the Window
 *
 */
void Game::Clean()
{
	Graphics::CloseWindow();
}

#pragma once

#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

#include "GameObject.h"
#include "StaticBody.h"
#include "Vector2.h"
#include "World.h"
#include "Collision.h"
#include "LevelBuilder.h"

class Game
{

public:
	static Game *instance();
	World *GetWorld() { return m_world; }
	void Init(const char *t_title, int t_xpos, int t_ypos, int t_width, int t_height, bool t_fullscreen);
	void HandleEvents();
	void Update(float t_dt);
	void Render();
	void Clean();

	// GETTERS
	bool Running() { return m_isRunning; }

private:
	World *m_world;
	LevelBuilder *m_level_builder;
	bool m_isRunning;

	// singleton
	static Game *m_uniqueInstance;
};

#endif
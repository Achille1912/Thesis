#pragma once

#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <vector>

#include "GameObject.h"
#include "StaticBody.h"
#include "Vector2.h"
#include "World.h"
#include "Collision.h"


class Game {

public:
	static Game* instance();
	World* getWorld() { return m_world; }
	void init(const char* t_title, int t_xpos, int t_ypos, int t_width, int t_height, bool t_fullscreen);
	void handleEvents();
	void update(float t_dt);
	void render();
	void clean();

	// GETTERS
	bool running() { return m_isRunning; }

private:
	World* m_world;
	bool m_isRunning;

	// singleton
	static Game* m_uniqueInstance;
	
};

#endif
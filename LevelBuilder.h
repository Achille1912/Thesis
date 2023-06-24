#pragma once
#include "World.h"
#include "DynamicBody.h"
#include "Constants.h"
#include "Graphics.h"

class LevelBuilder {
public:
	LevelBuilder (World* t_world);

	// Functions
	DynamicBody* LoadLevel(Levels t_level_name);

private:
	World* m_world;
};

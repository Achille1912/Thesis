#include "LevelBuilder.h"

LevelBuilder::LevelBuilder(World *t_world)
{
	m_world = t_world;
}

DynamicBody *LevelBuilder::LoadLevel(Levels t_level_name)
{
	switch (t_level_name)
	{
	case BASIC:
	{
		float mass2 = 100;
		m_world->AddGameObject(new DynamicBody("./gfx/woodBox.jpg", Graphics::renderer, 30, 10, Vector2f(0, 0), 46, 46, mass2, 0.4f));
		m_world->AddGameObject(new DynamicBody("./gfx/woodBox.jpg", Graphics::renderer, 110, 100, Vector2f(0, 0), 46, 46, mass2, 0.4f));
		m_world->AddGameObject(new StaticBody("./gfx/ground.png", Graphics::renderer, 10, 300, 300, 48, mass2, 20, 15, 0.4f));
		m_world->AddGameObject(new StaticBody("./gfx/ground.png", Graphics::renderer, 300, 450, 300, 48, mass2, 20, -15, 0.4f));
		m_world->AddGameObject(new StaticBody("./gfx/ground.png", Graphics::renderer, 10, 550, 600, 48, mass2, 20, 0, 0.4f));
		m_world->AddGameObject(new StaticBody("./gfx/ground.png", Graphics::renderer, 300, 80, 100, 48, mass2, 20, 0, 0.4f));

		return dynamic_cast<DynamicBody *>(m_world->GetGameObjects()[0]);
	}
	break;
	case ANGRY_BIRDS:
	{
		float mass2 = 100;
		m_world->AddGameObject(new DynamicBody("./gfx/bird.png", Graphics::renderer, 10, 490 - 70, Vector2f(0, 0), 46, 46, mass2, 0));
		m_world->AddGameObject(new DynamicBody("./gfx/woodBox.jpg", Graphics::renderer, 400, 390 - 70, Vector2f(0, 0), 26, 200, mass2, 0.4f));
		m_world->AddGameObject(new DynamicBody("./gfx/woodBox.jpg", Graphics::renderer, 600, 390 - 70, Vector2f(0, 0), 26, 200, mass2, 0.4f));
		m_world->AddGameObject(new DynamicBody("./gfx/woodBox.jpg", Graphics::renderer, 400, 290 - 70, Vector2f(0, 0), 226, 26, mass2, 0.4f));
		m_world->AddGameObject(new DynamicBody("./gfx/woodBox.jpg", Graphics::renderer, 450, 190 - 70, Vector2f(0, 0), 26, 150, mass2, 0.4f));
		m_world->AddGameObject(new DynamicBody("./gfx/woodBox.jpg", Graphics::renderer, 550, 190 - 70, Vector2f(0, 0), 26, 150, mass2, 0.4f));
		m_world->AddGameObject(new DynamicBody("./gfx/woodBox.jpg", Graphics::renderer, 450, 140 - 70, Vector2f(0, 0), 126, 26, mass2, 0.4f));
		m_world->AddGameObject(new DynamicBody("./gfx/woodBox.jpg", Graphics::renderer, 500, 90 - 70, Vector2f(0, 0), 26, 46, mass2, 0.4f));
		m_world->AddGameObject(new StaticBody("./gfx/nothing.png", Graphics::renderer, 10, 540 - 70, 800, 48, mass2, 20, 0, 0));

		return dynamic_cast<DynamicBody *>(m_world->GetGameObjects()[0]);
	}
	break;
	default:
		break;
	}
}
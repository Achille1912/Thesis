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
		m_world->AddGameObject(new DynamicBody("", Graphics::renderer, 30, 10, Vector2f(0, 0), 46, 60, mass2, 0.4f, {255,255,255,255}));
		m_world->AddGameObject(new DynamicBody("", Graphics::renderer, 110, 100, Vector2f(0, 0), 70, 46, mass2, 0.4f, { 255,255,255,255 }));
		m_world->AddGameObject(new StaticBody("", Graphics::renderer, 10, 300, 300, 48, mass2, 20, 15, 0.4f));
		m_world->AddGameObject(new StaticBody("", Graphics::renderer, 300, 450, 300, 48, mass2, 20, -15, 0.4f));
		m_world->AddGameObject(new StaticBody("", Graphics::renderer, 10, 550, 750, 48, mass2, 20, 0, 0.4f));
		m_world->AddGameObject(new StaticBody("", Graphics::renderer, 300, 80, 100, 48, mass2, 20, 0, 0.4f));
		m_world->AddGameObject(new StaticBody("", Graphics::renderer, -50, 0, 48, 600, mass2, 0, 0, 0.4f));
		m_world->AddGameObject(new StaticBody("", Graphics::renderer, 800, 0, 48, 600, mass2, 0, 0, 0.4f));
		return dynamic_cast<DynamicBody *>(m_world->GetGameObjects()[0]);
	}
	break;
	case ANGRY_BIRDS:
	{
		float mass2 = 10;
		m_world->AddGameObject(new DynamicBody("./gfx/bird.png", Graphics::renderer, 10, 490 - 70, Vector2f(0, 0), 64, 28, 100, 0, { 255,255,255,255 }));
		m_world->AddGameObject(new StaticBody("./gfx/woodBox.jpg", Graphics::renderer, 410, 370,  26, 100, mass2, 0.4f, 0, 0));
		m_world->AddGameObject(new StaticBody("./gfx/woodBox.jpg", Graphics::renderer, 590, 370,  26, 100, mass2, 0.4f, 0, 0));
		m_world->AddGameObject(new StaticBody("./gfx/woodBox.jpg", Graphics::renderer, 400, 344, 226, 26, mass2, 0.4f, 0, 0));
		

		m_world->AddGameObject(new DynamicBody("./gfx/woodBox.jpg", Graphics::renderer, 410, 320, Vector2f(0, 0), 23, 23, mass2, 0.4f));
		m_world->AddGameObject(new DynamicBody("./gfx/woodBox.jpg", Graphics::renderer, 444.5, 320, Vector2f(0, 0), 23, 23, mass2, 0.4f));
		m_world->AddGameObject(new DynamicBody("./gfx/woodBox.jpg", Graphics::renderer, 479, 320, Vector2f(0, 0), 23, 23, mass2, 0.4f));
		m_world->AddGameObject(new DynamicBody("./gfx/woodBox.jpg", Graphics::renderer, 513.5, 320, Vector2f(0, 0), 23, 23, mass2, 0.4f));
		m_world->AddGameObject(new DynamicBody("./gfx/woodBox.jpg", Graphics::renderer, 548, 320, Vector2f(0, 0), 23, 23, mass2, 0.4f));
		m_world->AddGameObject(new DynamicBody("./gfx/woodBox.jpg", Graphics::renderer, 582.5, 320, Vector2f(0, 0), 23, 23, mass2, 0.4f));
		
		m_world->AddGameObject(new DynamicBody("./gfx/woodBox.jpg", Graphics::renderer, 421, 297, Vector2f(0, 0), 23, 23, mass2, 0.4f));
		m_world->AddGameObject(new DynamicBody("./gfx/woodBox.jpg", Graphics::renderer, 456, 297, Vector2f(0, 0), 23, 23, mass2, 0.4f));
		m_world->AddGameObject(new DynamicBody("./gfx/woodBox.jpg", Graphics::renderer, 490.5, 297, Vector2f(0, 0), 23, 23, mass2, 0.4f));
		m_world->AddGameObject(new DynamicBody("./gfx/woodBox.jpg", Graphics::renderer, 525, 297, Vector2f(0, 0), 23, 23, mass2, 0.4f));
		m_world->AddGameObject(new DynamicBody("./gfx/woodBox.jpg", Graphics::renderer, 559, 297, Vector2f(0, 0), 23, 23, mass2, 0.4f));
		
		m_world->AddGameObject(new DynamicBody("./gfx/woodBox.jpg", Graphics::renderer, 435, 274, Vector2f(0, 0), 23, 23, mass2, 0.4f));
		m_world->AddGameObject(new DynamicBody("./gfx/woodBox.jpg", Graphics::renderer, 468, 274, Vector2f(0, 0), 23, 23, mass2, 0.4f));
		m_world->AddGameObject(new DynamicBody("./gfx/woodBox.jpg", Graphics::renderer, 504, 274, Vector2f(0, 0), 23, 23, mass2, 0.4f));
		m_world->AddGameObject(new DynamicBody("./gfx/woodBox.jpg", Graphics::renderer, 537, 274, Vector2f(0, 0), 23, 23, mass2, 0.4f));
		
		
		m_world->AddGameObject(new DynamicBody("./gfx/woodBox.jpg", Graphics::renderer, 444, 251, Vector2f(0, 0), 23, 23, mass2, 0.4f));
		m_world->AddGameObject(new DynamicBody("./gfx/woodBox.jpg", Graphics::renderer, 479, 251, Vector2f(0, 0), 23, 23, mass2, 0.4f));
		m_world->AddGameObject(new DynamicBody("./gfx/woodBox.jpg", Graphics::renderer, 513, 251, Vector2f(0, 0), 23, 23, mass2, 0.4f));
		
		m_world->AddGameObject(new DynamicBody("./gfx/woodBox.jpg", Graphics::renderer, 457, 228, Vector2f(0, 0), 23, 23, mass2, 0.4f));
		m_world->AddGameObject(new DynamicBody("./gfx/woodBox.jpg", Graphics::renderer, 491, 228, Vector2f(0, 0), 23, 23, mass2, 0.4f));
		
		



		
		
		m_world->AddGameObject(new StaticBody("./gfx/nothing.png", Graphics::renderer, 10, 540 - 70, 800, 48, mass2, 20, 0, 0));

		return dynamic_cast<DynamicBody *>(m_world->GetGameObjects()[0]);
	}
	break;
	case PEGGLE:
	{
		float mass2 = 100;
		m_world->AddGameObject(new DynamicBody("./gfx/mario.png", Graphics::renderer, 10, 490 - 70, Vector2f(0, 0), 46, 46, mass2, 0));
		
		for (int i = 0; i < 10; ++i) {
			m_world->AddGameObject(new StaticBody("./gfx/woodBox.jpg", Graphics::renderer, 20 + (i*80), 100, 10, 10, mass2, 20, 0, 0));
		}
		for (int i = 0; i < 10; ++i) {
			m_world->AddGameObject(new StaticBody("./gfx/woodBox.jpg", Graphics::renderer, (i * 80), 200, 10, 10, mass2, 20, 0, 0));
		}
		for (int i = 0; i < 10; ++i) {
			m_world->AddGameObject(new StaticBody("./gfx/woodBox.jpg", Graphics::renderer, 20 +(i * 80), 300, 10, 10, mass2, 20, 0, 0));
		}
		for (int i = 0; i < 10; ++i) {
			m_world->AddGameObject(new StaticBody("./gfx/woodBox.jpg", Graphics::renderer,  (i * 80), 400, 10, 10, mass2, 20, 0, 0));
		}

		return dynamic_cast<DynamicBody*>(m_world->GetGameObjects()[0]);
	}
	break;
	case STAIRS:
	{
		float mass2 = 100;
		m_world->AddGameObject(new DynamicBody("", Graphics::renderer, 10, 490 - 70, Vector2f(0, 0), 46, 46, mass2, 0));

		for (int i = 0; i < 10; ++i) {
			m_world->AddGameObject(new StaticBody("", Graphics::renderer, 20 + (i * 30), 100 + (i * 30), 30, 30, mass2, 20, 0, 0));
		}
		

		return dynamic_cast<DynamicBody*>(m_world->GetGameObjects()[0]);
	}
	break;
	default:
		break;
	}
}
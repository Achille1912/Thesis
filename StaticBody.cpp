#include "StaticBody.h"
#include "Vector2.h"
#include "Graphics.h"
#include "TextureManager.h"
#include "Game.h"
#include "Math.h"	

StaticBody::StaticBody(const char* t_textureSheet, SDL_Renderer* t_renderer, float t_xpos, float t_ypos, float t_width,
	float t_height, double t_mass, float t_friction, float t_angle, float t_restitution) 
	: GameObject(t_textureSheet, t_renderer, t_xpos, t_ypos, t_width, t_height, t_mass, t_angle, t_restitution, 0) {

	m_renderer = t_renderer;
	m_objectTexture = TextureManager::LoadTexture(t_textureSheet, t_renderer);

	m_restitution = t_restitution;

	m_dstRect.h = m_height;
	m_dstRect.w = m_width;
	m_theta = t_angle;

	m_dstRect.x = m_xpos;
	m_dstRect.y = m_ypos;

	m_vertices = CalculateVertices();
}





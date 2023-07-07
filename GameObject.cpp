#include <array>

#include "GameObject.h"
#include "Vector2.h"
#include "Graphics.h"
#include "Constants.h"
#include "Math.h"
#include "TextureManager.h"
#include "Game.h"

/**
 * @brief Construct a new Game Object
 *
 * @param t_textureSheet
 * @param t_renderer
 * @param t_xpos
 * @param t_ypos
 * @param t_width
 * @param t_height
 * @param t_mass
 * @param t_theta
 * @param t_restitution
 * @param t_invMass
 */
GameObject::GameObject(const char *t_textureSheet, SDL_Renderer *t_renderer, float t_xpos, float t_ypos,
					   float t_width, float t_height, float t_mass, float t_theta, float t_restitution, float t_invMass, SDL_Color t_border_color)
{

	m_renderer = t_renderer;
	m_objectTexture = TextureManager::LoadTexture(t_textureSheet, t_renderer);
	m_border_color = t_border_color;

	m_width = t_width;
	m_height = t_height;

	m_xpos = t_xpos;
	m_ypos = t_ypos;

	m_theta = t_theta;

	m_mass = t_mass;
	m_invMass = t_invMass;

	m_momentOfInertia = (m_mass * (m_width * m_width + m_height * m_height)) / 12;

	m_restitution = t_restitution;

	m_dstRect.h = t_height;
	m_dstRect.w = t_width;

	m_dstRect.x = m_xpos;
	m_dstRect.y = m_ypos;

	CalculateVertices();
	CalculateAABB();
}

/**
 * @brief Render Game Object
 *
 */
void GameObject::Render()
{
	if (m_objectTexture)
	{
		SDL_RenderCopyExF(m_renderer, m_objectTexture, NULL, &m_dstRect, m_theta, NULL, SDL_FLIP_NONE);
	}
	else
	{
		SDL_SetRenderDrawColor(m_renderer, m_border_color.r, m_border_color.g, m_border_color.b, m_border_color.a);
		for (int i = 0; i < 2; i++) 
		{
			for (int j = 0; j < 3; j++) 
			{
				SDL_RenderDrawLineF(m_renderer, m_vertices[j].x + i, m_vertices[j].y + i,
					m_vertices[j + 1].x + i, m_vertices[j + 1].y + i);
			}
			SDL_RenderDrawLineF(m_renderer, m_vertices[3].x + i, m_vertices[3].y + i,
				m_vertices[0].x + i, m_vertices[0].y + i);
		}
	}
	
}

/**
 * @brief Calculate Body's Vertices
 *
 */
void GameObject::CalculateVertices()
{
	// Top-Left
	m_vertices[0] = -Math::VectorRotation(Vector2f(m_xpos, m_ypos), GetCenter(), m_theta);
	// Top-Right
	m_vertices[1] = -Math::VectorRotation(Vector2f(m_xpos + m_width, m_ypos), GetCenter(), m_theta);
	// Bottom-Right
	m_vertices[2] = -Math::VectorRotation(Vector2f(m_xpos + m_width, m_ypos + m_height), GetCenter(), m_theta);
	// Bottom-Left
	m_vertices[3] = -Math::VectorRotation(Vector2f(m_xpos, m_ypos + m_height), GetCenter(), m_theta);
}

/**
 * @brief Calculate Body's AABB
 *
 */
void GameObject::CalculateAABB()
{
	float minX = INFINITY;
	float minY = INFINITY;
	float maxX = -INFINITY;
	float maxY = -INFINITY;

	for (const auto &v : m_vertices)
	{
		minX = std::min(minX, v.x);
		maxX = std::max(maxX, v.x);
		minY = std::min(minY, v.y);
		maxY = std::max(maxY, v.y);
	}

	m_aabb.x = minX;
	m_aabb.y = minY;
	m_aabb.w = maxX - minX;
	m_aabb.h = maxY - minY;
}
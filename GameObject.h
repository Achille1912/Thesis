#pragma once

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SDL2/SDL_render.h>
#include <array>

#include "Vector2.h"
#include "AABB.h"

class GameObject
{
public:
	GameObject(const char *t_textureSheet, SDL_Renderer *t_renderer, float t_xpos, float t_ypos, float t_width,
			   float t_height, float t_mass, float t_theta, float t_restitution, float t_invMass, SDL_Color t_border_color);
	virtual ~GameObject() {} // This virtual function allows me do the dynamic_cast's

	// SETTERS
	void SetCenterPosition(float t_xpos, float t_ypos)
	{
		m_xpos = t_xpos - m_width;
		m_ypos = t_ypos - m_height;
	}
	void SetPosition(Point t_pos)
	{
		m_xpos = t_pos.x;
		m_ypos = t_pos.y;
	}

	// GETTERS
	float GetX() { return m_xpos; }
	float GetY() { return m_ypos; }
	float GetW() { return m_width; }
	float GetH() { return m_height; }
	Point GetPos() { return Point(m_xpos, m_ypos); }
	Vector2f GetCenter() { return Vector2f(m_xpos + m_width / 2, m_ypos + m_height / 2); }
	float GetMass() { return m_mass; }
	SDL_FRect *GetRect() { return &m_dstRect; }
	SDL_Renderer *GetRenderer() { return m_renderer; }
	float GetMomentOfInertia() { return m_momentOfInertia; }
	SDL_Texture *GetTex() { return m_objectTexture; }
	float GetRotation() { return m_theta; }
	std::array<Vector2f, 4> GetVertices() { return m_vertices; }
	std::array<Vector2f, 4> GetNormals() { return m_normals; }
	float GetRestitution() { return m_restitution; }
	float GetInvMass() { return m_invMass; }
	AABB GetAABB() { return m_aabb; }
	bool GetIsStatic() { return m_is_static; }

	// Functions
	void CalculateVertices();
	void CalculateAABB();
	void Render();

protected:
	// Physics
	float m_xpos, m_ypos, m_width, m_height;
	float m_mass;
	float m_invMass;
	float m_momentOfInertia;
	float m_theta = 0;
	AABB m_aabb;
	float m_restitution;
	std::array<Vector2f, 4> m_vertices;
	std::array<Vector2f, 4> m_normals;
	bool m_is_static;

	// Rendering
	SDL_Texture *m_objectTexture;
	SDL_FRect m_dstRect;
	SDL_Renderer *m_renderer;
	SDL_Color m_border_color;
};

#endif
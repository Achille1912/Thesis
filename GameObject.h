#pragma once

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SDL_render.h>
#include <array>

#include "Vector2.h"
#include "AABB.h"

class GameObject {
public:
	GameObject(const char* t_textureSheet, SDL_Renderer* t_renderer, float t_xpos, float t_ypos, float t_width, 
		float t_height, float t_mass, float t_theta, float t_restitution, float t_invMass);
	virtual ~GameObject() {}

	// SETTERS
	void setCenterPosition(float t_xpos, float t_ypos) { m_xpos = t_xpos - m_width; m_ypos = t_ypos - m_height; }
	void setPosition(Point t_pos) { m_xpos = t_pos.x; m_ypos = t_pos.y; }

	// GETTERS
	float getX() { return m_xpos; }
	float getY() { return m_ypos; }
	float getW() { return m_width; }
	float getH() { return m_height; }
	Point getPos() { return Point(m_xpos, m_ypos); }
	Vector2f getCenter() { return Vector2f(m_xpos + m_width / 2, m_ypos + m_height / 2); }
	float getMass() { return m_mass; }
	SDL_FRect* getRect() { return &m_dstRect; }
	SDL_Renderer* getRenderer() { return m_renderer; }
	float getMomentOfInertia() { return m_momentOfInertia; }
	SDL_Texture* getTex() { return m_objectTexture; }
	float getRotation() { return m_theta; }
	std::array <Vector2f, 4> getVertices() { return m_vertices; }
	float getRestitution() { return m_restitution; }
	float getInvMass() { return m_invMass; }
	AABB GetAABB() { return m_aabb; }
	

	// Functions
	void CalculateVertices();
	void CalculateAABB();
	void render();


protected:
	// Physics
	float m_xpos, m_ypos, m_width, m_height;
	float m_mass;
	float m_invMass;
	float m_momentOfInertia;
	float m_theta = 0;
	AABB m_aabb;
	float m_restitution;
	std::array <Vector2f, 4> m_vertices;

	// Rendering
	SDL_Texture* m_objectTexture;
	SDL_FRect m_dstRect;
	SDL_Renderer* m_renderer;
};

#endif
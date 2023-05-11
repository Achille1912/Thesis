#pragma once

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H


#include "SDL_render.h"
#include <vector>
#include "Vector2.h"

class GameObject {
public:
	GameObject() {}
	GameObject(const char* t_textureSheet, SDL_Renderer* t_renderer, float t_xpos, float t_ypos, float t_width, float t_height, double t_mass, float theta);
	virtual ~GameObject() {}

	void render();

	/*SETTERS*/
	void setCenterPosition(float t_xpos, float t_ypos) { m_xpos = t_xpos - m_width; m_ypos = t_ypos - m_height; }
	void setPosition(Point t_pos) { m_xpos = t_pos.x; m_ypos = t_pos.y; }

	/*GETTERS*/
	float getX() { return m_xpos; }
	float getY() { return m_ypos; }
	float getW() { return m_width; }
	float getH() { return m_height; }
	Point getPos() { return Point(m_xpos, m_ypos); }
	Vector2f getCenter() { return Vector2f(m_xpos + m_width / 2, m_ypos + m_height / 2); }
	double getMass() { return m_mass; }
	float getInvMass() { return m_inv_mass; }
	SDL_Rect* getRect() { return &m_dstRect; }
	SDL_Renderer* getRenderer() { return m_renderer; }
	float getMomentOfInertia() { return m_momentOfInertia; }
	SDL_Texture* getTex() { return m_objectTexture; }
	float getRotation() { return m_theta; }
	std::vector <Vector2f> getVertices() { return m_vertices; }

	std::vector <Vector2f> CalculateVertices();

	


protected:
	float m_xpos, m_ypos, m_width, m_height;
	double m_mass;
	float m_inv_mass;
	float m_momentOfInertia;
	float m_theta = 0;

	SDL_Texture* m_objectTexture;
	SDL_Rect m_dstRect;

	SDL_Renderer* m_renderer;
	std::vector <Vector2f> m_vertices;

};

#endif
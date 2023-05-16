#pragma once

#ifndef STATICBODY_H
#define STATICBODY_H

#include "SDL_render.h"
#include <vector>
#include "GameObject.h"
#include "Vector2.h"

class StaticBody : public GameObject {
public:
	StaticBody(const char* t_textureSheet, SDL_Renderer* t_renderer, float t_xpos, float t_ypos, float t_width,
		float t_height, double t_mass, float t_friction, float t_angle, float t_restitution);

	// SETTERS
	void setCenterPosition(float t_xpos, float t_ypos) { m_xpos = t_xpos - m_width; m_ypos = t_ypos - m_height; }
	void setPosition(Point t_pos) { m_xpos = t_pos.x; m_ypos = t_pos.y; }


	// GETTERS
	float getX() { return m_xpos; }
	float getY() { return m_ypos; }
	float getW() { return m_width; }
	float getH() { return m_height; }
	Point getPos() { return Point(m_xpos, m_ypos); }
	Vector2f getCenter() { return Vector2f(m_xpos + m_width/2, m_ypos + m_height/2); }
	double getMass() { return m_mass; }
	SDL_FRect* getRect() { return &m_dstRect; }
	SDL_Texture* getTex() { return m_objectTexture; }

	std::vector <Vector2f> m_vertices;
	
private:
	// Rendering
	SDL_Texture* m_objectTexture;
	SDL_FRect m_dstRect;
	SDL_Renderer* m_renderer;
};

#endif
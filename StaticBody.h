#pragma once

#ifndef STATICBODY_H
#define STATICBODY_H

#include <SDL_render.h>
#include <array>

#include "GameObject.h"
#include "Vector2.h"

class StaticBody : public GameObject
{
public:
	StaticBody(const char *t_textureSheet, SDL_Renderer *t_renderer, float t_xpos, float t_ypos, float t_width,
			   float t_height, double t_mass, float t_friction, float t_angle, float t_restitution);

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
};

#endif
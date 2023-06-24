#pragma once

#ifndef DYNAMICBODY_H
#define DYNAMICBODY_H

#include <SDL_render.h>
#include <vector>
#include <tuple>

#include "GameObject.h"
#include "Collision.h"
#include "Vector2.h"

class DynamicBody : public GameObject
{

public:
	DynamicBody(const char *t_textureSheet, SDL_Renderer *t_renderer, float t_xpos, float t_ypos,
				Vector2f t_velocity, float t_width, float t_height, float t_mass, float t_restitution);

	// SETTERS
	void SetVelocity(Vector2f t_velocity) { m_velocity = t_velocity; }
	void SetVelocityX(float t_x) { m_velocity.x = t_x; }
	void SetVelocityY(float t_y) { m_velocity.y = t_y; }
	void SetMoving(bool t_m) { m_moving = t_m; }
	void SetAngularVelocity(float t_angularVelocity) { m_angularVelocity = t_angularVelocity; }

	// GETTERS
	Vector2f GetVelocity() { return Vector2f(m_velocity.x, m_velocity.y); }
	bool GetMoving() { return m_moving; }
	float GetAngularVelocity() { return m_angularVelocity; }

	// Functions
	void Update(float t_dt, float t_gravity, int t_iterations);
	void Move(Vector2f t_dir);
	void Rotate();
	void MoveTo(Vector2f t_pos);
	void MoveBy(float t_xpos, float t_ypos);
	void AddVel(Vector2f t_value) { m_velocity += t_value; }

private:
	// Physics
	Vector2f m_velocity;
	float m_momentOfInertia;
	float m_angularAcceleration;
	float m_angularVelocity;
	bool m_moving;
};
#endif

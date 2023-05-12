#pragma once

#ifndef DYNAMICBODY_H
#define DYNAMICBODY_H

#include "SDL_render.h"
#include <vector>
#include <tuple>

#include "GameObject.h"
#include "Collision.h"
#include "Vector2.h"

class DynamicBody: public GameObject {

public:
	DynamicBody(const char* t_textureSheet, SDL_Renderer* t_renderer, float t_xpos, float t_ypos, 
		Vector2f t_velocity, float t_width, float t_height, double t_mass, float t_restitution);

	// SETTERS
	void setVelocity(Vector2f t_velocity) { m_velocity = t_velocity; }
	void setVelocityX(float t_x) { m_velocity.x = t_x; }
	void setVelocityY(float t_y) { m_velocity.y = t_y; }
	void setMoving(bool t_m) { m_moving = t_m; }
	void setCenterPosition(float t_xpos, float t_ypos) { m_xpos = t_xpos - m_width; m_ypos = t_ypos - m_height; }
	void setPosition(Point t_pos) { m_xpos = t_pos.x; m_ypos = t_pos.y; }
	void setOmega(float t_omega) { m_omega = t_omega; }
	void setAngularVelocity(float t_angularVelocity) { m_angularVelocity = t_angularVelocity; }

	// GETTERS
	Vector2f getVelocity() { return Vector2f(m_velocity.x, m_velocity.y); }
	float getX() { return m_xpos; }
	float getY() { return m_ypos; }
	float getW() { return m_width; }
	float getH() { return m_height; }
	bool getMoving() { return m_moving; }
	Point getPos() { return Point(m_xpos, m_ypos); }
	Vector2f getCenter() { return Vector2f(m_xpos + m_width/2, m_ypos + m_height/2); }
	double getMass() { return m_mass; }
	SDL_Rect* getRect() { return &m_dstRect; }
	SDL_Texture* getTex() { return m_objectTexture; }
	float getRotation() { return m_theta; }
	float getOmega() { return m_omega; }
	float getAngularVelocity() { return m_angularVelocity; }

	// Functions
	void update(float t_dt, float t_gravity, int t_iterations);
	void move(Vector2f t_dir);
	void rotate();
	void moveTo(Vector2f t_pos);
	void moveBy(float t_xpos, float t_ypos);
	void AddVel(Vector2f t_value) { m_velocity = m_velocity + t_value; }
	
private:
	// Physics
	Vector2f m_velocity;
	float m_momentOfInertia;
	float m_omega;
	float m_mass;
	float m_angularAcceleration = 0;
	float m_angularVelocity = 0;
	bool m_moving = false;

	// Rendering
	SDL_Texture* m_objectTexture;
	SDL_Renderer* m_renderer;
};
#endif 

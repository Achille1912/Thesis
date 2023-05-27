#include "DynamicBody.h"
#include "Vector2.h"
#include "TextureManager.h"
#include "Game.h"
#include "Utils.h"
#include "Graphics.h"
#include "Constants.h"
#include "Math.h"


DynamicBody::DynamicBody(const char* t_textureSheet, SDL_Renderer* t_renderer, float t_xpos, float t_ypos,
	Vector2f t_velocity, float t_width, float t_height, float t_mass, float t_restitution)
	:GameObject(t_textureSheet, t_renderer, t_xpos, t_ypos, t_width,t_height, t_mass, 0, t_restitution, 1/t_mass) {

	m_renderer = t_renderer;
	m_objectTexture = TextureManager::LoadTexture(t_textureSheet, t_renderer);
	m_velocity = t_velocity;
	m_angularVelocity = 0;
	m_angularAcceleration = 0;
	m_moving = false;
	m_momentOfInertia =  (m_mass * (m_width * m_width + m_height * m_height)) / 12;	

}

void DynamicBody::update(float t_dt, float t_gravity, int t_iterations) {

	t_dt /= t_iterations;

	// Euler's Integration
	m_velocity +=  Vector2f(0,t_gravity * t_dt);
	m_xpos += m_velocity.x * t_dt;
	m_ypos += m_velocity.y * t_dt;

	m_theta += m_angularVelocity * t_dt* ANGULAR_FACTOR ;

	// New Position Rect
	Utils::SetRectSizeAndPosition(m_dstRect, m_xpos, m_ypos, m_width, m_height);

	// Dumb friction
	if (m_velocity.x < 0) m_velocity.x += (1.5 * t_dt * PIXELS_PER_METER);
	if (m_velocity.x > 0) m_velocity.x -= (1.5 * t_dt * PIXELS_PER_METER);

	// Calculate New Vertices
	CalculateVertices();
}

void DynamicBody::move(Vector2f t_direction) {
	m_xpos += t_direction.x;
	m_ypos += t_direction.y;
}

void DynamicBody::moveBy(float t_xpos, float t_ypos) {
	m_xpos += t_xpos;
	m_ypos += t_ypos;

}

void DynamicBody::moveTo(Vector2f t_position) {
	m_xpos = t_position.x;
	m_ypos = t_position.y;
}

void DynamicBody::rotate() {
	m_theta += 10;

	CalculateVertices();
}


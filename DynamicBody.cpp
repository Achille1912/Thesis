#include "DynamicBody.h"
#include "Vector2.h"
#include "TextureManager.h"
#include "Game.h"
#include "Utils.h"
#include "Graphics.h"
#include "Constants.h"
#include "Math.h"


DynamicBody::DynamicBody(const char* t_textureSheet, SDL_Renderer* t_renderer, float t_xpos, float t_ypos,
	Vector2f t_velocity, float t_width, float t_height, double t_mass, float t_restitution)
	:GameObject(t_textureSheet, t_renderer, t_xpos, t_ypos, t_width,t_height, t_mass, 0) {
	m_renderer = t_renderer;
	m_objectTexture = TextureManager::LoadTexture(t_textureSheet, t_renderer);
	m_velocity = t_velocity;
	m_angularVelocity = 0;
	m_momentOfInertia =  (m_mass * (m_width * m_width + m_height * m_height)) / 12;
	m_restitution = t_restitution;

	
}

void DynamicBody::update(float t_dt, float t_gravity, int t_iterations) {
	t_dt /= t_iterations;

	// Euler's Integration
	m_velocity +=  Vector2f(0,t_gravity * t_dt);
	m_xpos += m_velocity.x * t_dt;
	m_ypos += + m_velocity.y * t_dt;

	m_theta += m_angularVelocity * t_dt* ANGULAR_FACTOR ;

	// New Position Rect
	Utils::SetRectSizeAndPosition(m_dstRect, m_xpos, m_ypos, m_width, m_height);

	m_vertices = CalculateVertices();

	// Safe Area
	if (std::abs(m_velocity.len()) < SmallAmount) {
		m_velocity = Vector2f(0, 0);
	}

}

void DynamicBody::render() {
	
	
	if (Graphics::debug) {
		
		SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 0);
		SDL_RenderFillRect(m_renderer, &m_dstRect);
		SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);

		Graphics::DrawVertices(dynamic_cast<GameObject*>(this), Color(0, 255, 0, 0));
	}
	SDL_RenderCopyEx(m_renderer, m_objectTexture, NULL, &m_dstRect, m_theta, NULL, SDL_FLIP_NONE);

	if (Graphics::debug) {
		Graphics::DrawCenterPoint(dynamic_cast<GameObject*>(this), 5, 5, Color(0, 0, 255, 0));
	}

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

	m_vertices = CalculateVertices();
}


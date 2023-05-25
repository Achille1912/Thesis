#include <array>

#include "GameObject.h"
#include "Vector2.h"
#include "Graphics.h"
#include "Constants.h"
#include "Math.h"
#include "TextureManager.h"
#include "Game.h"

GameObject::GameObject(const char* t_textureSheet, SDL_Renderer* t_renderer, float t_xpos, float t_ypos, 
	float t_width, float t_height, float t_mass, float t_theta, float t_restitution, float t_invMass) {

	m_renderer = t_renderer;
	m_objectTexture = TextureManager::LoadTexture(t_textureSheet, t_renderer);

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

	m_vertices = CalculateVertices();
}

void GameObject::render() {
	if (Graphics::debug) {
		SDL_FRect tmpRect = GetAABB();
		SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 0);
		SDL_RenderFillRectF(m_renderer, &tmpRect);
		SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
	}
	
	SDL_RenderCopyExF(m_renderer, m_objectTexture, NULL, &m_dstRect, m_theta, NULL, SDL_FLIP_NONE);
	SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 0);
	

	if (Graphics::debug) Graphics::DrawCenterPoint(dynamic_cast<GameObject*>(this), 5, 5, Color(0, 0, 255, 0));
}


std::array <Vector2f,4> GameObject::CalculateVertices() {
	std::array <Vector2f,4> result;

	float angle = this->getRotation();

	Vector2f up_left_final = Math::VectorRotation(Vector2f(this->getX(), this->getY()), this->getCenter(), angle);

	Vector2f up_right_final = Math::VectorRotation(Vector2f(this->getX() + this->getW(), this->getY()), this->getCenter(), angle);

	Vector2f down_right_final = Math::VectorRotation(Vector2f(this->getX() + this->getW(), this->getY() + this->getH()), this->getCenter(), angle);

	Vector2f down_left_final = Math::VectorRotation(Vector2f(this->getX(), this->getY() + this->getH()), this->getCenter(), angle);

	result[0] = (Vector2f(-up_left_final.x, -up_left_final.y));
	result[1] = (Vector2f(-up_right_final.x, -up_right_final.y));
	result[2] = (Vector2f(-down_right_final.x, -down_right_final.y));
	result[3] = (Vector2f(-down_left_final.x, -down_left_final.y));

	return result;
}


SDL_FRect GameObject::GetAABB() {

	SDL_FRect res;
	float minX = INFINITY;
	float minY = INFINITY;
	float maxX = -INFINITY;
	float maxY = -INFINITY;

	for (int i = 0; i < m_vertices.size(); i++) {
			Vector2f v = m_vertices[i];

			if (v.x < minX) { minX = v.x; }
			if (v.x > maxX) { maxX = v.x; }
			if (v.y < minY) { minY = v.y; }
			if (v.y > maxY) { maxY = v.y; }
		}
	
	res.x = minX;
	res.y = minY;
	res.w = maxX - minX;
	res.h = maxY - minY;

	return res;
}
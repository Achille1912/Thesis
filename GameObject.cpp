#include "GameObject.h"
#include "Vector2.h"
#include "Graphics.h"
#include <vector>
#include "Constants.h"
#include "Math.h"
#include "TextureManager.h"
#include "Game.h"

GameObject::GameObject(const char* t_textureSheet, SDL_Renderer* t_renderer, float t_xpos, float t_ypos, 
	float t_width, float t_height, double t_mass, float t_theta, float t_restitution, float t_invMass) {

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
		SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 0);
		SDL_RenderFillRect(m_renderer, &m_dstRect);
		SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
		Graphics::DrawVertices(m_renderer, CalculateVertices(), Color(0, 255, 0, 0));
	}

	SDL_RenderCopyEx(m_renderer, m_objectTexture, NULL, &m_dstRect, m_theta, NULL, SDL_FLIP_NONE);

	if (Graphics::debug) Graphics::DrawCenterPoint(dynamic_cast<GameObject*>(this), 5, 5, Color(0, 0, 255, 0));
}


std::vector <Vector2f> GameObject::CalculateVertices() {
	std::vector <Vector2f> result;

	float angle = this->getRotation();

	Vector2f up_left_final = Math::VectorRotation(Vector2f(this->getX(), this->getY()), this->getCenter(), angle);

	Vector2f up_right_final = Math::VectorRotation(Vector2f(this->getX() + this->getW(), this->getY()), this->getCenter(), angle);

	Vector2f down_right_final = Math::VectorRotation(Vector2f(this->getX() + this->getW(), this->getY() + this->getH()), this->getCenter(), angle);

	Vector2f down_left_final = Math::VectorRotation(Vector2f(this->getX(), this->getY() + this->getH()), this->getCenter(), angle);

	result.push_back(Vector2f(-up_left_final.x, -up_left_final.y));
	result.push_back(Vector2f(-up_right_final.x, -up_right_final.y));
	result.push_back(Vector2f(-down_right_final.x, -down_right_final.y));
	result.push_back(Vector2f(-down_left_final.x, -down_left_final.y));

	return result;
}
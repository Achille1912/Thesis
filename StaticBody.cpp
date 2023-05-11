#include "StaticBody.h"
#include "Vector2.h"
#include "Graphics.h"
#include "TextureManager.h"
#include "Game.h"
#include "Math.h"	

StaticBody::StaticBody(const char* t_textureSheet, SDL_Renderer* t_renderer, float t_xpos, float t_ypos, float t_width,
	float t_height, double t_mass, float t_friction, float t_angle, float t_restitution) 
	: GameObject(t_textureSheet, t_renderer, t_xpos, t_ypos, t_width, t_height, t_mass, t_angle) {
	m_renderer = t_renderer;
	m_objectTexture = TextureManager::LoadTexture(t_textureSheet, t_renderer);

	m_restitution = t_restitution;
	m_friction = t_friction;

	m_dstRect.h = m_height;
	m_dstRect.w = m_width;
	m_theta = t_angle;

	m_dstRect.x = m_xpos;
	m_dstRect.y = m_ypos;


	m_vertices = CalculateVertices();
}

void StaticBody::render() {
	// Seleziona il colore rosso per il punto
	SDL_SetRenderDrawColor(Graphics::renderer, 255, 0, 0, 255);

	// Disegna un punto nella posizione (320, 240)
	SDL_RenderDrawPoint(Graphics::renderer, 10, 10);
	SDL_Rect center;


	center.w = 5;
	center.h = 5;

	center.x = m_xpos + m_width / 2 - center.w / 2;
	center.y = m_ypos + m_height / 2 - center.h / 2;

	if (Graphics::debug) {
		SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 0);
		SDL_RenderFillRect(m_renderer, &m_dstRect);
		SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
		SDL_SetRenderDrawColor(Graphics::renderer, 0, 255, 0, 255);
		for (int i = 0; i < m_vertices.size(); i++) {
				Graphics::SDL_RenderDrawCircle(Graphics::renderer, m_vertices[i].x, m_vertices[i].y, 5);
			}
			SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);

		}
		SDL_RenderCopyEx(m_renderer, m_objectTexture, NULL, &m_dstRect, m_theta, NULL, SDL_FLIP_NONE);

		if (Graphics::debug) {
			SDL_SetRenderDrawColor(m_renderer, 0, 0, 255, 0);
			SDL_RenderFillRect(m_renderer, &center);
			SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
		}
}


std::vector <Vector2f> StaticBody::CalculateVertices() {
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


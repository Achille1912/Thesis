#include "StaticBody.h"
#include "Vector2.h"
#include "Graphics.h"
#include "TextureManager.h"
#include "Game.h"
#include "Math.h"
/**
 * @brief Construct a new Static Body
 *
 * @param t_textureSheet
 * @param t_renderer
 * @param t_xpos
 * @param t_ypos
 * @param t_width
 * @param t_height
 * @param t_mass
 * @param t_friction
 * @param t_angle
 * @param t_restitution
 */
StaticBody::StaticBody(const char *t_textureSheet, SDL_Renderer *t_renderer, float t_xpos, float t_ypos, float t_width,
					   float t_height, double t_mass, float t_friction, float t_angle, float t_restitution, SDL_Color t_border_color)
	: GameObject(t_textureSheet, t_renderer, t_xpos, t_ypos, t_width, t_height, t_mass, t_angle, t_restitution, 0, t_border_color)
{
}

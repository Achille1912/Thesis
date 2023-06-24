#pragma once

#ifndef AABB_H
#define AABB_H

struct AABB
{
	AABB() : x(0.0f), y(0.0f), w(0.0f), h(0.0f) {}

	AABB(float t_x, float t_y, float t_w, float t_h) : x(t_x), y(t_y), w(t_w), h(t_h) {}

	float x, y, w, h;
};

#endif
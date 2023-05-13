#pragma once

#ifndef VECTOR2_H
#define VECTOR2_H


#include <iostream>


class Point {
public:
	Point(){}
	Point(float t_x, float t_y) {
		x = t_x;
		y = t_y;
	}
	float x;
	float y;
};


struct Vector2f
{
	Vector2f() :x(0.0f), y(0.0f) {}

	Vector2f(float t_x, float t_y) : x(t_x), y(t_y) {}

	float len() { return std::sqrt( x * x + y * y); }

	Vector2f vers() {
		if (x == 0 && y == 0) {
			return Vector2f(0, 0);
		}
		else {
			float len = Vector2f(x, y).len();
			return Vector2f(x / len, y / len);
		}
	}

	void operator=(Vector2f t_vector) {
		x = t_vector.x;
		y = t_vector.y;
	}

	Vector2f operator-(Vector2f t_vector) {
		return Vector2f(x - t_vector.x, y - t_vector.y);
	}

	Vector2f operator/(double t_value) {
		return Vector2f(x / t_value, y / t_value);
	}

	Vector2f operator*(float t_value) {
		return Vector2f(x * t_value, y * t_value);
	}


	Vector2f operator+(Vector2f t_vector) {
		return Vector2f(x + t_vector.x, y + t_vector.y);
	}

	Vector2f& operator+=(const Vector2f& t_vector) {
		x += t_vector.x;
		y += t_vector.y;
		return *this;
	}

	Vector2f& operator+=(float t_value) {
		x += t_value;
		y += t_value;
		return *this;
	}

	Vector2f& operator-=(const Vector2f& t_vector) {
		x -= t_vector.x;
		y -= t_vector.y;
		return *this;
	}

	Vector2f& operator-=(float t_value) {
		x -= t_value;
		y -= t_value;
		return *this;
	}

	friend std::ostream& operator<<(std::ostream& os, const Vector2f& t_vector) {
		os << "(" << t_vector.x << ", " << t_vector.y << ")" << std::endl;
		return os;
	}


	Vector2f getDistance(Vector2f t_vector) {
		return Vector2f(x - t_vector.x, y - t_vector.y);
	}

	



	float x, y;
};


#endif
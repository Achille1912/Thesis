#pragma once

#ifndef COLLISION_H
#define COLLISION_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>

#include "Vector2.h"
#include "GameObject.h"

enum CollisionMode { DynVsDyn, DynVsStatic, StaticVsDyn };


struct PointToSegmentType {
public:
	PointToSegmentType() {
	}

	float distSq;
	Vector2f closestPoint;
};


struct ContactType {
public:
	ContactType(Vector2f c1, Vector2f c2, int cn) {
		contact1 = c1;
		contact2 = c2;
		contactNumber = cn;
	}

	Vector2f contact1;
	Vector2f contact2;
	int contactNumber;
};


struct CollisionType {
public:
	CollisionType() {
	}

	CollisionType(float d, Vector2f ax, CollisionMode m) {
		depth = d;
		CollidingAxis = Vector2f(ax.x, ax.y);
		mode = m;
	}

	float depth;
	Vector2f CollidingAxis;
	bool areColliding;
	CollisionMode mode;
};




struct Collision {
	
	static CollisionType IntersectSAT(GameObject* objA, GameObject* objB);

	static ContactType FindContactPoints(GameObject* objA, GameObject* objB);

	static bool IntersectAABB(AABB AABBobjA, AABB AABBobjB);

};

#endif
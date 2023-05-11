#pragma once

#include "SDL.h"
#include "SDL_image.h"
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

	GameObject* obj;

	float getDepth(){ return depth; }

	void setDepth(float value) { depth = value; }

	bool getAreColliding() { return areColliding; }

	void setAreColliding(bool value) { areColliding = value; }

	Vector2f getCollidingAxis() { return CollidingAxis; }

	void setCollidingAxis(Vector2f ax) { CollidingAxis = ax; }

	void setCollisionMode(CollisionMode m) { mode = m; }

	CollisionMode getCollisionMode() { return mode; }

private:
	float depth;
	Vector2f CollidingAxis;
	bool areColliding;
	CollisionMode mode;
};





class Collision {
public:
	CollisionType IntersectSAT(GameObject* objA, GameObject* objB);

	ContactType FindContactPoints(GameObject* objA, GameObject* objB);

private:
};
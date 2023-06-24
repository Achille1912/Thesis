#pragma once

#ifndef WORLD_H
#define WORLD_H

#include <vector>

#include "GameObject.h"
#include "DynamicBody.h"
#include "StaticBody.h"
#include "Vector2.h"
#include "Collision.h"

class World
{
private:
	float m_G = 9.81f;
	std::vector<GameObject *> m_objects;

public:
	World(float t_gravity, bool t_basic);
	~World();

	void AddGameObject(GameObject *t_obj);

	void DeleteObject(GameObject *t_obj);

	std::vector<GameObject *> &GetGameObjects();

	void Update(float t_dt, int t_iterationNumber);

	void CheckCollisions();

	void SolveDynVsDynCollisionBasic(DynamicBody *t_objA, DynamicBody *t_objB, CollisionType t_check);

	void SolveDynVsStaticCollisionBasic(DynamicBody *t_objA, StaticBody *t_objB, CollisionType t_check);

	void SolveDynVsDynCollisionRotation(DynamicBody *t_objA, DynamicBody *t_objB, CollisionType t_check);

	void SolveDynVsStaticCollisionRotation(DynamicBody *t_objA, StaticBody *t_objB, CollisionType t_check);

	bool m_basic;

	std::array<Vector2f, 2> m_contactList;
};

#endif
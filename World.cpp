#include "World.h"
#include "DynamicBody.h"
#include "StaticBody.h"
#include "Collision.h"
#include "Constants.h"
#include "Graphics.h"
#include "Math.h"
#include "Utils.h"
#include "timeUtils.h"
#include "array"

/**
 * @brief Create a new World
 *
 * @param t_gravity
 * @param t_basic
 */
World::World(float t_gravity, bool t_basic)
{
	m_G = -t_gravity * PIXELS_PER_METER;
	m_basic = t_basic;
}

/**
 * @brief Destroy the World
 *
 */
World::~World()
{
	for (auto obj : m_objects)
	{
		delete obj;
	}
}

/**
 * @brief Add a Game Object in the World
 *
 * @param obj
 */
void World::AddGameObject(GameObject *obj)
{
	m_objects.push_back(obj);
}

/**
 * @brief Delete a Game Object from the World
 *
 * @param obj
 */
void World::DeleteObject(GameObject *obj)
{
	auto it = std::find(m_objects.begin(), m_objects.end(), obj);
	if (it != m_objects.end())
	{
		delete *it;
		m_objects.erase(it);
	}
}

/**
 * @brief Get the Objects Vector
 *
 * @return std::vector<GameObject*>&
 */
std::vector<GameObject *> &World::GetGameObjects()
{
	return m_objects;
}

/**
 * @brief Check if there are Collisions
 *
 */
void World::CheckCollisions()
{
	/************SAT COLLISION DETECTION******************/
	for (int i = 0; i < m_objects.size(); ++i)
	{
		for (int j = i + 1; j < m_objects.size(); ++j)
		{
			// If Both Obj are Static
			if (Utils::isInstanceOf(m_objects[i], typeid(StaticBody)) && Utils::isInstanceOf(m_objects[j], typeid(StaticBody)))
			{
				continue;
			}
			// If AABBs don't intersect, jump to the next check
			if (!Collision::IntersectAABB(m_objects[i]->GetAABB(), m_objects[j]->GetAABB()))
			{
				continue;
			}
			CollisionType check = Collision::IntersectSAT(m_objects[i], m_objects[j]);
			if (check.areColliding)
			{
				switch (check.mode)
				{
				case DynVsDyn:
				{
					DynamicBody *objA = dynamic_cast<DynamicBody *>(m_objects[i]);
					DynamicBody *objB = dynamic_cast<DynamicBody *>(m_objects[j]);

					// Move the Rectangles by the depth collision
					objA->MoveBy(-check.CollidingAxis.x * check.depth / 2, -check.CollidingAxis.y * check.depth / 2);
					objB->MoveBy(check.CollidingAxis.x * check.depth / 2, check.CollidingAxis.y * check.depth / 2);

					// Resolve Collision
					if (m_basic)
						SolveDynVsDynCollisionBasic(objA, objB, check);
					else
						SolveDynVsDynCollisionRotation(objA, objB, check);

					break;
				}
				case DynVsStatic:
				{
					DynamicBody *objA = dynamic_cast<DynamicBody *>(m_objects[i]);
					StaticBody *objB = dynamic_cast<StaticBody *>(m_objects[j]);

					// Move the Rectangles by the depth collision
					objA->MoveBy(-check.CollidingAxis.x * check.depth, -check.CollidingAxis.y * check.depth);

					// Resolve Collision
					if (m_basic)
						SolveDynVsStaticCollisionBasic(objA, objB, check);
					else
						SolveDynVsStaticCollisionRotation(objA, objB, check);

					break;
				}
				case StaticVsDyn:
				{
					StaticBody *objA = dynamic_cast<StaticBody *>(m_objects[i]);
					DynamicBody *objB = dynamic_cast<DynamicBody *>(m_objects[j]);

					// Move the Rectangles by the depth collision
					objB->MoveBy(check.CollidingAxis.x * check.depth, check.CollidingAxis.y * check.depth);

					// Resolve Collision
					if (m_basic)
						SolveDynVsStaticCollisionBasic(objB, objA, check);
					else
						SolveDynVsStaticCollisionRotation(objB, objA, check);

					break;
				}
				default:
					break;
				}
			}
		}
	}
}

/**
 * @brief Solve Collisions between Dynamic Objects
 *
 * @param t_objA
 * @param t_objB
 * @param t_check
 */
void World::SolveDynVsDynCollisionRotation(DynamicBody *t_objA, DynamicBody *t_objB, CollisionType t_check)
{

	ContactType result = Collision::FindContactPoints(t_objA, t_objB);

	Vector2f normal = t_check.CollidingAxis;

	float e = std::min(t_objA->GetRestitution(), t_objB->GetRestitution());

	m_contactList[0] = (result.contact1);
	m_contactList[1] = (result.contact2);

	for (int i = 0; i < result.contactNumber; i++)
	{
		// Calculate Impulses
		Vector2f ra = m_contactList[i] - t_objA->GetCenter();
		Vector2f rb = m_contactList[i] - t_objB->GetCenter();

		Vector2f raPerp(-ra.y, ra.x);
		Vector2f rbPerp(-rb.y, rb.x);

		Vector2f angularLinearVelocityA = raPerp * t_objA->GetAngularVelocity();
		Vector2f angularLinearVelocityB = rbPerp * t_objB->GetAngularVelocity();

		Vector2f vrel = (t_objA->GetVelocity() + angularLinearVelocityA) -
						(t_objB->GetVelocity() + angularLinearVelocityB);

		float contactVelocityMag = Math::Dot(vrel, normal);

		float raPerpDotN = Math::Dot(raPerp, normal);
		float rbPerpDotN = Math::Dot(rbPerp, normal);

		float denom = t_objA->GetInvMass() + t_objB->GetInvMass() +
					  (raPerpDotN * raPerpDotN) / (t_objA->GetMomentOfInertia()) +
					  (rbPerpDotN * rbPerpDotN) / (t_objB->GetMomentOfInertia());

		float j = -(1 + e) * contactVelocityMag;

		j /= denom;

		j /= result.contactNumber;

		Vector2f impulse = normal * j;

		// Apply Impulses
		t_objA->SetVelocity(t_objA->GetVelocity() + impulse * t_objA->GetInvMass());

		t_objA->SetAngularVelocity(t_objA->GetAngularVelocity() +
								   Math::Cross(ra, impulse) / t_objA->GetMomentOfInertia());

		t_objB->SetVelocity(t_objB->GetVelocity() - impulse * t_objB->GetInvMass());

		t_objB->SetAngularVelocity(t_objB->GetAngularVelocity() +
								   Math::Cross(rb, -impulse) / t_objB->GetMomentOfInertia());
	}
}

/**
 * @brief Solve Collisions between Dynamic and Static Objects
 *
 * @param t_objA
 * @param t_objB
 * @param t_check
 */
void World::SolveDynVsStaticCollisionRotation(DynamicBody *t_objA, StaticBody *t_objB, CollisionType t_check)
{

	ContactType result = Collision::FindContactPoints(t_objA, t_objB);

	Vector2f normal = t_check.CollidingAxis;
	float e = std::min(t_objA->GetRestitution(), t_objB->GetRestitution());

	m_contactList[0] = (result.contact1);
	m_contactList[1] = (result.contact2);

	for (int i = 0; i < result.contactNumber; i++)
	{
		// Calculate Impulses
		Vector2f ra = m_contactList[i] - t_objA->GetCenter();
		Vector2f rb = m_contactList[i] - t_objB->GetCenter();

		Vector2f raPerp(-ra.y, ra.x);
		Vector2f rbPerp(-rb.y, rb.x);

		Vector2f angularLinearVelocityA = raPerp * t_objA->GetAngularVelocity();

		Vector2f vrel = (t_objA->GetVelocity() + angularLinearVelocityA);

		float contactVelocityMag = Math::Dot(vrel, normal);

		float raPerpDotN = Math::Dot(raPerp, normal);
		float rbPerpDotN = Math::Dot(rbPerp, normal);

		float denom = t_objA->GetInvMass() + 0 +
					  (raPerpDotN * raPerpDotN) / (t_objA->GetMomentOfInertia());

		float j = -(1 + e) * contactVelocityMag;

		j /= denom;

		j /= result.contactNumber;

		Vector2f impulse = normal * j;

		// Apply Impulses

		t_objA->SetVelocity(t_objA->GetVelocity() + impulse * t_objA->GetInvMass());

		t_objA->SetAngularVelocity(t_objA->GetAngularVelocity() +
								   Math::Cross(ra, impulse) / t_objA->GetMomentOfInertia());
	}
}

/**
 * @brief Update the Physic of all World's Objects
 *
 * @param t_dt
 * @param t_iterationNumber
 */
void World::Update(float t_dt, int t_iterationNumber)
{
	for (int it = 0; it < t_iterationNumber; it++)
	{
		for (auto obj : m_objects)
		{
			if (Utils::isInstanceOf(obj, typeid(DynamicBody)))
			{
				DynamicBody *dynamic_obj = dynamic_cast<DynamicBody *>(obj);
				dynamic_obj->Update(t_dt, m_G, t_iterationNumber);
				if (dynamic_obj->GetY() > 700)
					DeleteObject(obj);
			}
		}
		CheckCollisions();
	}
}

/**
 * @brief Solve Collisions between Dynamic Objects in a Basic Way
 *
 * @param t_objA
 * @param t_objB
 * @param t_check
 */
void World::SolveDynVsDynCollisionBasic(DynamicBody *t_objA, DynamicBody *t_objB, CollisionType t_check)
{
	Vector2f normal = t_check.CollidingAxis;

	float e = std::min(t_objA->GetRestitution(), t_objB->GetRestitution());

	Vector2f vrel = (t_objA->GetVelocity()) - (t_objB->GetVelocity());

	Vector2f impulse = Math::BasicResponseImpulse(e, vrel, normal, t_objA->GetInvMass(), t_objB->GetInvMass());

	t_objA->SetVelocity(t_objA->GetVelocity() + impulse * t_objA->GetInvMass());

	t_objB->SetVelocity(t_objB->GetVelocity() - impulse * t_objB->GetInvMass());
}

/**
 * @brief  * @brief Solve Collisions between Dynamic and Static Objects in a Basic Way
 * @param t_objA
 * @param t_objB
 * @param t_check
 */
void World::SolveDynVsStaticCollisionBasic(DynamicBody *t_objA, StaticBody *t_objB, CollisionType t_check)
{
	Vector2f normal = t_check.CollidingAxis;

	float e = std::min(t_objA->GetRestitution(), t_objB->GetRestitution());

	Vector2f vrel = (t_objA->GetVelocity());

	Vector2f impulse = Math::BasicResponseImpulse(e, vrel, normal, t_objA->GetInvMass(), t_objB->GetInvMass());

	t_objA->SetVelocity(t_objA->GetVelocity() + impulse * t_objA->GetInvMass());
}

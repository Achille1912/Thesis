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


World::World(float t_gravity, bool t_basic) {
	m_G = -t_gravity * PIXELS_PER_METER;
	m_basic = t_basic;
	m_collisionManager = new Collision();
}

World::~World() {
	for (auto obj : m_objects) {
		delete obj;
	}
}

void World::AddGameObject(GameObject* obj) {
	m_objects.push_back(obj);
}

void World::DeleteObject(GameObject* obj) {
	delete obj;
}

std::vector<GameObject*>& World::GetGameObjects() {
	return m_objects;
}


void World::CheckCollisions() {
	/************SAT COLLISION DETECTION******************/
	for (int i = 0; i < m_objects.size(); ++i) {
		for (int j = i + 1; j < m_objects.size(); ++j) {
			// If Both Obj are Static
			if (Utils::isInstanceOf(m_objects[i], typeid(StaticBody)) && Utils::isInstanceOf(m_objects[j], typeid(StaticBody))) {
				continue;
			}
			// Idk why if i use AABB check first, the complexity increase
			if (m_collisionManager->IntersectAABB(m_objects[i]->GetAABB(), m_objects[j]->GetAABB())) {
				CollisionType check = m_collisionManager->IntersectSAT(m_objects[i], m_objects[j]);
				if (check.areColliding) {
					// Evidence the Hitting Boxes
					if (Graphics::debug) {
						SDL_SetTextureColorMod(m_objects[i]->getTex(), 255, 0, 0);
						SDL_SetTextureColorMod(m_objects[j]->getTex(), 255, 0, 0);
					}
					switch (check.mode) {
						case DynVsDyn: {
							DynamicBody* objA = dynamic_cast<DynamicBody*>(m_objects[i]);
							DynamicBody* objB = dynamic_cast<DynamicBody*>(m_objects[j]);

							// Move the Rectangles by the depth collision
							objA->moveBy(-check.CollidingAxis.x * check.depth / 2, -check.CollidingAxis.y * check.depth / 2);
							objB->moveBy(check.CollidingAxis.x * check.depth / 2, check.CollidingAxis.y * check.depth / 2);
							if (m_basic) SolveDynVsDynCollisionBasic(objA, objB, check);
							else SolveDynVsDynCollisionRotation(objA, objB, check);

							break;
						}
						case DynVsStatic: {
							DynamicBody* objA = dynamic_cast<DynamicBody*>(m_objects[i]);
							StaticBody* objB = dynamic_cast<StaticBody*>(m_objects[j]);

							// Move the Rectangles by the depth collision
							objA->moveBy(-check.CollidingAxis.x * check.depth, -check.CollidingAxis.y * check.depth);

	
							if (m_basic) SolveDynVsStaticCollisionBasic(objA, objB, check);
							else SolveDynVsStaticCollisionRotation(objA, objB, check);

							break;
						}
						case StaticVsDyn: {
							StaticBody* objA = dynamic_cast<StaticBody*>(m_objects[i]);
							DynamicBody* objB = dynamic_cast<DynamicBody*>(m_objects[j]);

							// Move the Rectangles by the depth collision
							objB->moveBy(check.CollidingAxis.x * check.depth, check.CollidingAxis.y * check.depth);


							if (m_basic) SolveDynVsStaticCollisionBasic(objB, objA, check);
							else SolveDynVsStaticCollisionRotation(objB, objA, check);

							break;
						}
						default:
							break;
					}
				}
			}
		}
	}
}


void World::SolveDynVsDynCollisionRotation(DynamicBody* t_objA, DynamicBody* t_objB, CollisionType t_check) {

	ContactType result = m_collisionManager->FindContactPoints(t_objA, t_objB);

	if (Graphics::debug) {
		SDL_SetRenderDrawColor(Graphics::renderer, 255, 0, 0, 255);
		Graphics::SDL_RenderDrawCircle(Graphics::renderer, result.contact1.x, result.contact1.y, 5);
		Graphics::SDL_RenderDrawCircle(Graphics::renderer, result.contact2.x, result.contact2.y, 5);
		Graphics::RenderFrame();
	}

	Vector2f normal = t_check.CollidingAxis;

	float e =  std::min(t_objA->getRestitution(), t_objB->getRestitution());


	std::array <Vector2f, 2> contactList;
	std::array <Vector2f,2> impulseList;
	std::array <Vector2f, 2> raList;
	std::array <Vector2f, 2> rbList;

	contactList[0] = (result.contact1);
	contactList[1] = (result.contact2);


	// Calculate Impulses
	for (int i = 0; i < result.contactNumber; i++) {
		Vector2f ra = contactList[i] - t_objA->getCenter() ;
		Vector2f rb = contactList[i] - t_objB->getCenter();

		raList[i] = ra;
		rbList[i] = rb;

		Vector2f raPerp = Vector2f(-ra.y, ra.x);
		Vector2f rbPerp = Vector2f(-rb.y, rb.x);

		Vector2f angularLinearVelocityA = raPerp * t_objA->getAngularVelocity();
		Vector2f angularLinearVelocityB = rbPerp * t_objB->getAngularVelocity();


		Vector2f vrel = (t_objA->getVelocity() + angularLinearVelocityA) -
			(t_objB->getVelocity() + angularLinearVelocityB);


		float contactVelocityMag = Math::Dot(vrel, normal);
	
		float raPerpDotN = Math::Dot(raPerp, normal);
		float rbPerpDotN = Math::Dot(rbPerp, normal);

		float denom = t_objA->getInvMass() + t_objB->getInvMass() +
			(raPerpDotN * raPerpDotN) / (t_objA->getMomentOfInertia()) +
			(rbPerpDotN * rbPerpDotN) / (t_objB->getMomentOfInertia());

		float j = -(1 + e) * contactVelocityMag;

		j /= denom;

		j /= result.contactNumber;

		Vector2f impulse =  normal * j;
		
		impulseList[i] =impulse;

	}
	
	// Apply Impulses
	for (int i = 0; i < result.contactNumber; i++) {
		
		Vector2f impulse = impulseList[i];
		

		Vector2f ra = raList[i];
		Vector2f rb = rbList[i];


		t_objA->setVelocity(t_objA->getVelocity() + impulse * t_objA->getInvMass());

		t_objA->setAngularVelocity(t_objA->getAngularVelocity() +
			Math::Cross(ra, impulse) / t_objA->getMomentOfInertia());

		t_objB->setVelocity(t_objB->getVelocity() - impulse * t_objB->getInvMass());

		t_objB->setAngularVelocity(t_objB->getAngularVelocity() +
			Math::Cross(rb, -impulse) / t_objB->getMomentOfInertia());

	}

}


void World::SolveDynVsStaticCollisionRotation(DynamicBody* t_objA, StaticBody* t_objB, CollisionType t_check) {
	
	ContactType result = m_collisionManager->FindContactPoints(t_objA, t_objB);

	if (Graphics::debug) {
		SDL_SetRenderDrawColor(Graphics::renderer, 255, 0, 0, 255);
		Graphics::SDL_RenderDrawCircle(Graphics::renderer, result.contact1.x, result.contact1.y, 5);
		Graphics::SDL_RenderDrawCircle(Graphics::renderer, result.contact2.x, result.contact2.y, 5);
		Graphics::RenderFrame();
	}

	Vector2f normal = t_check.CollidingAxis;
	float e = std::min(t_objA->getRestitution(), t_objB->getRestitution());

	std::array <Vector2f, 2> contactList;
	std::array <Vector2f,2> impulseList;
	std::array <Vector2f, 2> raList;
	std::array <Vector2f, 2> rbList;

	contactList[0] = (result.contact1);
	contactList[1] = (result.contact2);

	// Calculate Impulses
	for (int i = 0; i < result.contactNumber; i++) {
		Vector2f ra = contactList[i] - t_objA->getCenter();
		Vector2f rb = contactList[i] - t_objB->getCenter();

		raList[i] = ra;
		rbList[i] = rb;

		Vector2f raPerp = Vector2f(-ra.y, ra.x);
		Vector2f rbPerp = Vector2f(-rb.y, rb.x);

		Vector2f angularLinearVelocityA = raPerp * t_objA->getAngularVelocity();


		Vector2f vrel = (t_objA->getVelocity() + angularLinearVelocityA);


		float contactVelocityMag = Math::Dot(vrel, normal);

		float raPerpDotN = Math::Dot(raPerp, normal);
		float rbPerpDotN = Math::Dot(rbPerp, normal);

		float denom = t_objA->getInvMass() + 0 +
			(raPerpDotN * raPerpDotN) / (t_objA->getMomentOfInertia());

		float j = -(1 + e) * contactVelocityMag;

		j /= denom;

		j /= result.contactNumber;

		
		Vector2f impulse = normal * j;

		impulseList[i] = impulse;

	}
	
	// Apply Impulses
	for (int i = 0; i < result.contactNumber; i++) {

		Vector2f impulse = impulseList[i];

		Vector2f ra = raList[i];
		Vector2f rb = rbList[i];

		t_objA->setVelocity(t_objA->getVelocity() + impulse * t_objA->getInvMass());

		t_objA->setAngularVelocity(t_objA->getAngularVelocity() +
			Math::Cross(ra, impulse) / t_objA->getMomentOfInertia());
	}
}

void World::Update(float t_dt, int t_iterationNumber) {
	for (int it = 0; it < t_iterationNumber; it++) {
		for (auto obj : m_objects) {
			if (Utils::isInstanceOf(obj, typeid(DynamicBody))) {
				DynamicBody* dynamic_obj = dynamic_cast<DynamicBody*>(obj);
				dynamic_obj->update(t_dt, m_G, t_iterationNumber);
			}
		}
		CheckCollisions();
	}
	
}


void World::SolveDynVsDynCollisionBasic(DynamicBody* t_objA, DynamicBody* t_objB, CollisionType t_check) {
	Vector2f normal = t_check.CollidingAxis;

	float e = std::min(t_objA->getRestitution(), t_objB->getRestitution());

	Vector2f vrel = (t_objA->getVelocity()) - (t_objB->getVelocity());

	Vector2f impulse = Math::BasicResponseImpulse(e, vrel, normal, t_objA->getInvMass(), t_objB->getInvMass());


	t_objA->setVelocity(t_objA->getVelocity() + impulse * t_objA->getInvMass());

	t_objB->setVelocity(t_objB->getVelocity() - impulse * t_objB->getInvMass());

}


void World::SolveDynVsStaticCollisionBasic(DynamicBody* t_objA, StaticBody* t_objB, CollisionType t_check) {
	Vector2f normal = t_check.CollidingAxis;

	float e = std::min(t_objA->getRestitution(), t_objB->getRestitution());

	Vector2f vrel = (t_objA->getVelocity());

	Vector2f impulse = Math::BasicResponseImpulse(e, vrel, normal, t_objA->getInvMass(), t_objB->getInvMass());

	t_objA->setVelocity(t_objA->getVelocity() + impulse * t_objA->getInvMass());
}

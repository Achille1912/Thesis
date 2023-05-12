#include "World.h"
#include "DynamicBody.h"
#include "StaticBody.h"
#include "Collision.h"
#include "Constants.h"
#include "Graphics.h"
#include "Math.h"
#include "Utils.h"


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
			if (!(typeid(*m_objects[i]) == typeid(StaticBody) && typeid(*m_objects[j]) == typeid(StaticBody))) {
				CollisionType check = m_collisionManager->IntersectSAT(m_objects[i], m_objects[j]);
				if (check.getAreColliding()) {

					// Evidence the Hitting Boxes
					if (Graphics::debug) {
						SDL_SetTextureColorMod(m_objects[i]->getTex(), 255, 0, 0);
						SDL_SetTextureColorMod(m_objects[j]->getTex(), 255, 0, 0);
					}
					switch (check.getCollisionMode())
					{
					case DynVsDyn:
					{
						DynamicBody* objA = dynamic_cast<DynamicBody*>(m_objects[i]);
						DynamicBody* objB = dynamic_cast<DynamicBody*>(m_objects[j]);

						// Move the Rectangles by the depth collision
						objA->moveBy(-check.getCollidingAxis().x * check.getDepth() / 2, -check.getCollidingAxis().y * check.getDepth() / 2);
						objB->moveBy(check.getCollidingAxis().x * check.getDepth() / 2, check.getCollidingAxis().y * check.getDepth() / 2);
						if(m_basic) SolveDynVsDynCollisionBasic(objA, objB, check);
						else SolveDynVsDynCollisionRotationAndFriction(objA, objB, check);

						break;
					}
					case DynVsStatic:
					{
						DynamicBody* objA = dynamic_cast<DynamicBody*>(m_objects[i]);
						StaticBody* objB = dynamic_cast<StaticBody*>(m_objects[j]);

						// Move the Rectangles by the depth collision
						objA->moveBy(-check.getCollidingAxis().x * check.getDepth(), -check.getCollidingAxis().y * check.getDepth());


						if(m_basic) SolveStaticVsDynCollisionBasic(objA, objB, check);
						else SolveStaticVsDynCollisionRotationAndFriction(objA, objB, check);

						break;
					}
					case StaticVsDyn:
					{
						StaticBody* objA = dynamic_cast<StaticBody*>(m_objects[i]);
						DynamicBody* objB = dynamic_cast<DynamicBody*>(m_objects[j]);

						// Move the Rectangles by the depth collision
						objB->moveBy(check.getCollidingAxis().x * check.getDepth(), check.getCollidingAxis().y * check.getDepth());


						if(m_basic) SolveDynVsStaticCollisionBasic(objB, objA, check);
						else SolveDynVsStaticCollisionRotationAndFriction(objB, objA, check);

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

void World::SolveDynVsDynCollisionRotationAndFriction(DynamicBody* t_objA, DynamicBody* t_objB, CollisionType t_check) {

	ContactType result = m_collisionManager->FindContactPoints(t_objA, t_objB);

	if (Graphics::debug) {
		SDL_SetRenderDrawColor(Graphics::renderer, 255, 0, 0, 255);
		Graphics::SDL_RenderDrawCircle(Graphics::renderer, result.contact1.x, result.contact1.y, 5);
		Graphics::SDL_RenderDrawCircle(Graphics::renderer, result.contact2.x, result.contact2.y, 5);
		Graphics::RenderFrame();
	}

	Vector2f normal = t_check.getCollidingAxis();

	float e =  std::min(t_objA->getRestitution(), t_objB->getRestitution());

	float sf = (t_objA->getStaticFriction() + t_objB->getStaticFriction()) * 0.5;
	float df = (t_objA->getDynamicFriction() + t_objB->getDynamicFriction()) * 0.5;

	std::vector <Vector2f> contactList;
	std::vector <Vector2f> impulseList(2);
	std::vector <Vector2f> frictionImpulseList(2);
	std::vector <float> jList(2);
	std::vector <Vector2f> raList(2);
	std::vector <Vector2f> rbList(2);

	contactList.push_back(result.contact1);
	contactList.push_back(result.contact2);

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

		jList[i] = j;
		Vector2f impulse =  normal * j;
		
		impulseList[i] =impulse;

	}
	/******************************************/
	for (int i = 0; i < result.contactNumber; i++) {
		
		Vector2f impulse = impulseList[i];
		

		Vector2f ra = raList[i];
		Vector2f rb = rbList[i];


		t_objA->setVelocity(t_objA->getVelocity() + impulse * t_objA->getInvMass());

		t_objA->setAngularVelocity(t_objA->getAngularVelocity() +
			Math::Cross(ra, impulse) / t_objA->getMomentOfInertia());

		t_objB->setVelocity(t_objB->getVelocity() - impulse * t_objB->getInvMass());

		t_objB->setAngularVelocity(t_objB->getAngularVelocity() -
			Math::Cross(rb, impulse) / t_objB->getMomentOfInertia());

	}
	/*****************friction**************/
	for (int i = 0; i < result.contactNumber; i++) {
		Vector2f ra = contactList[i] - t_objA->getCenter();
		Vector2f rb = contactList[i] - t_objB->getCenter();

		raList[i] = ra;
		rbList[i] = rb;



		Vector2f raPerp = Vector2f(-ra.y, ra.x);
		Vector2f rbPerp = Vector2f(-rb.y, rb.x);


		Vector2f angularLinearVelocityA = raPerp * t_objA->getAngularVelocity();
		Vector2f angularLinearVelocityB = rbPerp * t_objB->getAngularVelocity();


		Vector2f vrel = (t_objA->getVelocity() + angularLinearVelocityA) -
			(t_objB->getVelocity() + angularLinearVelocityB);

		Vector2f tangent = vrel - ( normal * Math::Dot(vrel, normal));

		if (Math::NearlyEqual(tangent, Vector2f(0, 0))) {
			continue;
		}
		else {
			tangent = tangent.vers();
		}


		float raPerpDotT = Math::Dot(raPerp, tangent);
		float rbPerpDotT = Math::Dot(rbPerp, tangent);

		float denom = t_objA->getInvMass() + t_objB->getInvMass() +
			(raPerpDotT * raPerpDotT) / (t_objA->getMomentOfInertia()) +
			(rbPerpDotT * rbPerpDotT) / (t_objB->getMomentOfInertia());

		float jt = - Math::Dot(vrel, tangent);
		

		jt /= denom;

		jt /= result.contactNumber;

		Vector2f frictionImpulse;
		float j = jList[i];
		if (std::abs(jt) <= j * sf) {
			frictionImpulse = tangent * jt;
		}
		else {
			frictionImpulse = tangent * -j * df;
		}

		frictionImpulseList[i] = frictionImpulse;

	}
	/******************************************/
	for (int i = 0; i < result.contactNumber; i++) {

		Vector2f frictionImpulse = frictionImpulseList[i];


		Vector2f ra = raList[i];
		Vector2f rb = rbList[i];

		t_objA->setVelocity(t_objA->getVelocity() + frictionImpulse * t_objA->getInvMass());

		t_objA->setAngularVelocity(t_objA->getAngularVelocity() +
			Math::Cross(ra, frictionImpulse) / t_objA->getMomentOfInertia());

		t_objB->setVelocity(t_objB->getVelocity() - frictionImpulse * t_objB->getInvMass());

		t_objB->setAngularVelocity(t_objB->getAngularVelocity() -
			Math::Cross(rb, frictionImpulse) / t_objB->getMomentOfInertia());

	}

}


void World::SolveStaticVsDynCollisionRotationAndFriction(DynamicBody* t_objA, StaticBody* t_objB, CollisionType t_check) {


	ContactType result = m_collisionManager->FindContactPoints(t_objA, t_objB);

	if (Graphics::debug) {
		SDL_SetRenderDrawColor(Graphics::renderer, 255, 0, 0, 255);
		Graphics::SDL_RenderDrawCircle(Graphics::renderer, result.contact1.x, result.contact1.y, 5);
		Graphics::SDL_RenderDrawCircle(Graphics::renderer, result.contact2.x, result.contact2.y, 5);
		Graphics::RenderFrame();
	}

	Vector2f normal = t_check.getCollidingAxis();
	float e = std::min(t_objA->getRestitution(), t_objB->getRestitution());
	float sf = (t_objA->getStaticFriction() + t_objB->getStaticFriction()) * 0.5;
	float df = (t_objA->getDynamicFriction() + t_objB->getDynamicFriction()) * 0.5;

	std::vector <Vector2f> contactList;
	std::vector <Vector2f> impulseList(2);
	std::vector <Vector2f> frictionImpulseList(2);
	std::vector <float> jList(2);
	std::vector <Vector2f> raList(2);
	std::vector <Vector2f> rbList(2);

	contactList.push_back(result.contact1);
	contactList.push_back(result.contact2);



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

		jList[i] = j;
		Vector2f impulse = normal * j;

		impulseList[i] = impulse;

	}
	/******************************************/
	for (int i = 0; i < result.contactNumber; i++) {

		Vector2f impulse = impulseList[i];


		Vector2f ra = raList[i];
		Vector2f rb = rbList[i];

		t_objA->setVelocity(t_objA->getVelocity() + impulse * t_objA->getInvMass());

		t_objA->setAngularVelocity(t_objA->getAngularVelocity() +
			Math::Cross(ra, impulse) / t_objA->getMomentOfInertia());

	}

	/*****************friction**************/
	for (int i = 0; i < result.contactNumber; i++) {
		Vector2f ra = contactList[i] - t_objA->getCenter();
		Vector2f rb = contactList[i] - t_objB->getCenter();

		raList[i] = ra;
		rbList[i] = rb;



		Vector2f raPerp = Vector2f(-ra.y, ra.x);
		Vector2f rbPerp = Vector2f(-rb.y, rb.x);


		Vector2f angularLinearVelocityA = raPerp * t_objA->getAngularVelocity();



		Vector2f vrel = (t_objA->getVelocity() + angularLinearVelocityA);

		Vector2f tangent = vrel - (normal * Math::Dot(vrel, normal));

		if (Math::NearlyEqual(tangent, Vector2f(0, 0))) {
			continue;
		}
		else {
			tangent = tangent.vers();
		}


		float raPerpDotT = Math::Dot(raPerp, tangent);
		float rbPerpDotT = Math::Dot(rbPerp, tangent);

		float denom = t_objA->getInvMass() +
			(raPerpDotT * raPerpDotT) / (t_objA->getMomentOfInertia());

		float jt = -Math::Dot(vrel, tangent);


		jt /= denom;

		jt /= result.contactNumber;

		Vector2f frictionImpulse;
		float j = jList[i];
		if (std::abs(jt) <= j * sf) {
			frictionImpulse = tangent * jt;
		}
		else {
			frictionImpulse = tangent * -j * df;
		}

		frictionImpulseList[i] = frictionImpulse *-1;

	}
	/******************************************/
	for (int i = 0; i < result.contactNumber; i++) {

		Vector2f frictionImpulse = frictionImpulseList[i];


		Vector2f ra = raList[i];
		Vector2f rb = rbList[i];

		t_objA->setVelocity(t_objA->getVelocity() + frictionImpulse * t_objA->getInvMass());

		t_objA->setAngularVelocity(t_objA->getAngularVelocity() +
			Math::Cross(ra, frictionImpulse) / t_objA->getMomentOfInertia());
	}
	
}


void World::SolveDynVsStaticCollisionRotationAndFriction(DynamicBody* t_objA, StaticBody* t_objB, CollisionType t_check) {
	
	ContactType result = m_collisionManager->FindContactPoints(t_objA, t_objB);

	if (Graphics::debug) {
		SDL_SetRenderDrawColor(Graphics::renderer, 255, 0, 0, 255);
		Graphics::SDL_RenderDrawCircle(Graphics::renderer, result.contact1.x, result.contact1.y, 5);
		Graphics::SDL_RenderDrawCircle(Graphics::renderer, result.contact2.x, result.contact2.y, 5);
		Graphics::RenderFrame();
	}

	Vector2f normal = t_check.getCollidingAxis();
	float e = std::min(t_objA->getRestitution(), t_objB->getRestitution());
	float sf = (t_objA->getStaticFriction() + t_objB->getStaticFriction()) * 0.5;
	float df = (t_objA->getDynamicFriction() + t_objB->getDynamicFriction()) * 0.5;

	std::vector <Vector2f> contactList;
	std::vector <Vector2f> impulseList(2);
	std::vector <Vector2f> frictionImpulseList(2);
	std::vector <float> jList(2);
	std::vector <Vector2f> raList(2);
	std::vector <Vector2f> rbList(2);

	contactList.push_back(result.contact1);
	contactList.push_back(result.contact2);


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

		jList[i] = j;
		Vector2f impulse = normal * j;

		impulseList[i] = impulse;

	}
	/******************************************/
	for (int i = 0; i < result.contactNumber; i++) {

		Vector2f impulse = impulseList[i];


		Vector2f ra = raList[i];
		Vector2f rb = rbList[i];

		t_objA->setVelocity(t_objA->getVelocity() + impulse * t_objA->getInvMass());

		t_objA->setAngularVelocity(t_objA->getAngularVelocity() +
			Math::Cross(ra, impulse) / t_objA->getMomentOfInertia());
	}

	/*****************friction**************/
	for (int i = 0; i < result.contactNumber; i++) {
		Vector2f ra = contactList[i] - t_objA->getCenter();
		Vector2f rb = contactList[i] - t_objB->getCenter();

		raList[i] = ra;
		rbList[i] = rb;



		Vector2f raPerp = Vector2f(-ra.y, ra.x);
		Vector2f rbPerp = Vector2f(-rb.y, rb.x);


		Vector2f angularLinearVelocityA = raPerp * t_objA->getAngularVelocity();



		Vector2f vrel = (t_objA->getVelocity() + angularLinearVelocityA);

		Vector2f tangent = vrel - (normal * Math::Dot(vrel, normal));

		if (Math::NearlyEqual(tangent, Vector2f(0, 0))) {
			continue;
		}
		else {
			tangent = tangent.vers();
		}


		float raPerpDotT = Math::Dot(raPerp, tangent);
		float rbPerpDotT = Math::Dot(rbPerp, tangent);

		float denom = t_objA->getInvMass() +
			(raPerpDotT * raPerpDotT) / (t_objA->getMomentOfInertia());

		float jt = -Math::Dot(vrel, tangent);


		jt /= denom;

		jt /= result.contactNumber;

		Vector2f frictionImpulse;
		float j = jList[i];
		if (std::abs(jt) <= j * sf) {
			frictionImpulse = tangent * jt;
		}
		else {
			frictionImpulse = tangent * -j * df;
		}

		frictionImpulseList[i] = frictionImpulse;

	}
	/******************************************/
	for (int i = 0; i < result.contactNumber; i++) {

		Vector2f frictionImpulse = frictionImpulseList[i];


		Vector2f ra = raList[i];
		Vector2f rb = rbList[i];

		t_objA->setVelocity(t_objA->getVelocity() + frictionImpulse * t_objA->getInvMass());

		t_objA->setAngularVelocity(t_objA->getAngularVelocity() +
			Math::Cross(ra, frictionImpulse) / t_objA->getMomentOfInertia());

	}


}

void World::Update(float t_dt, int t_iterationNumber) {
	for (int it = 0; it < t_iterationNumber; it++) {
		for (auto obj : m_objects) {
			if (obj != nullptr) {
				if (typeid(*obj) == typeid(DynamicBody)) {
					DynamicBody* dynamic_obj = dynamic_cast<DynamicBody*>(obj);
					dynamic_obj->update(t_dt, m_G, t_iterationNumber);

				}
			}
		}

		CheckCollisions();

		for (auto obj : m_objects) {
			if (obj->getY() > 600) {
				m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), obj), m_objects.end());
			}
		}
	}
}


void World::SolveDynVsDynCollisionBasic(DynamicBody* t_objA, DynamicBody* t_objB, CollisionType t_check) {
	Vector2f normal = t_check.getCollidingAxis();

	float e = std::min(t_objA->getRestitution(), t_objB->getRestitution());

	Vector2f vrel = (t_objA->getVelocity()) - (t_objB->getVelocity());

	Vector2f impulse = Math::BasicResponseImpulse(e, vrel, normal, t_objA->getInvMass(), t_objB->getInvMass());

	t_objA->setVelocity(t_objA->getVelocity() + impulse * t_objA->getInvMass());

	t_objB->setVelocity(t_objB->getVelocity() - impulse * t_objB->getInvMass());

}

void World::SolveStaticVsDynCollisionBasic(DynamicBody* t_objA, StaticBody* t_objB, CollisionType t_check) {
	Vector2f normal = t_check.getCollidingAxis();

	float e = std::min(t_objA->getRestitution(), t_objB->getRestitution());

	Vector2f vrel = (t_objA->getVelocity());

	Vector2f impulse = Math::BasicResponseImpulse(e, vrel, normal, t_objA->getInvMass(), 0);

	t_objA->setVelocity(t_objA->getVelocity() + impulse * t_objA->getInvMass());
}

void World::SolveDynVsStaticCollisionBasic(DynamicBody* t_objA, StaticBody* t_objB, CollisionType t_check) {
	Vector2f normal = t_check.getCollidingAxis();

	float e = std::min(t_objA->getRestitution(), t_objB->getRestitution());

	Vector2f vrel = (t_objA->getVelocity());

	Vector2f impulse = Math::BasicResponseImpulse(e, vrel, normal, t_objA->getInvMass(), 0);

	t_objA->setVelocity(t_objA->getVelocity() + impulse * t_objA->getInvMass());
}

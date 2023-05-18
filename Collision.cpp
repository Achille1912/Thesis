#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <cmath>

#include "Vector2.h"
#include "Collision.h"
#include "DynamicBody.h"
#include "StaticBody.h"
#include "Game.h"
#include "Utils.h"
#include "Math.h"

PointToSegmentType PointToSegmentDistance(Vector2f p, Vector2f va, Vector2f vb) {

	PointToSegmentType result;

	Vector2f ab = vb - va;

	Vector2f ap = p - va;

	float proj = Math::Dot(ap, ab);
	float abLenSq = (ab.x * ab.x + ab.y * ab.y);
	float d = proj / abLenSq;

	if (d <= 0) result.closestPoint = va;
	
	else if (d >= 1) result.closestPoint = vb;

	else result.closestPoint = va + ab * d;
	
	result.distSq = Math::getDistanceSquared(p,result.closestPoint);

	return result;
}


Vector2f CalculateNormal(Vector2f pointA, Vector2f pointB) {
	Vector2f directionVector = pointB - pointA;

	return Vector2f(directionVector.y, -directionVector.x);
}

std::vector <Vector2f> CalculateNormals(std::vector <Vector2f>& vertices) {
	std::vector <Vector2f> normals(vertices.size());

	for (int i = 0; i < vertices.size(); i++) {
		normals[i] = CalculateNormal(vertices[i + 1 % vertices.size()], vertices[i]);
	}
	normals[vertices.size() - 1] = CalculateNormal(vertices[0], vertices[vertices.size() - 1]);
	return normals;
}


CollisionType Collision::IntersectSAT(GameObject* objA, GameObject* objB) {

	CollisionType result;
	Vector2f result_normal = Vector2f(0, 0);
	float depth = INFINITY;

	std::vector <Vector2f> verticesA;
	
	std::vector <Vector2f> verticesB;

	verticesA = objA->CalculateVertices();	

	verticesB = objB->CalculateVertices();
	

	std::vector <Vector2f> normalsA = CalculateNormals(verticesA);
	std::vector <Vector2f> normalsB = CalculateNormals(verticesB);

	std::vector <Vector2f> normals;
	normals.insert(normals.end(), normalsA.begin(), normalsA.end());
	normals.insert(normals.end(), normalsB.begin(), normalsB.end());

	
	for (Vector2f normal : normals) {
		float minA = INFINITY;
		float maxA = -INFINITY;
		for (Vector2f vertex : verticesA) {
			float proj = Math::Dot(vertex,normal);
			minA = std::min(minA, proj);
			maxA = std::max(maxA, proj);

		}
		float minB = INFINITY;
		float maxB = -INFINITY;
		for (Vector2f vertex : verticesB) {
			float proj = Math::Dot(vertex, normal);
			minB = std::min(minB, proj);
			maxB = std::max(maxB, proj);

		}

		if (maxA < minB || maxB < minA) {
			
			result.areColliding =false;
			result.CollidingAxis = Vector2f(0, 0);
			result.depth = 0;
			return result;
		}
		float axisDepth = (std::min(maxB - minA, maxA - minB));
		if (axisDepth < depth) {
			depth = axisDepth;
			result_normal = normal;
		}
	}
	depth = depth / result_normal.len();
	result_normal = result_normal.vers();

	Vector2f centerA = Math::FindArithmeticMean(verticesA);
	Vector2f centerB = Math::FindArithmeticMean(verticesB);

	Vector2f direction = centerB - centerA;

	if (Math::Dot(direction,result_normal) < 0) {
		result_normal = Vector2f(-result_normal.x, -result_normal.y);
	}

	
	result.areColliding = true;
	result.depth = depth;
	result.CollidingAxis = result_normal;

	


	// Check Collision Mdoe
	
	if (Utils::isInstanceOf(objA, typeid(DynamicBody)) && 
		Utils::isInstanceOf(objB, typeid(DynamicBody))) {
		result.mode = DynVsDyn;
	}
	else if (Utils::isInstanceOf(objA, typeid(DynamicBody)) &&
		Utils::isInstanceOf(objB, typeid(StaticBody))) {
		result.mode = DynVsStatic;
	}
	else if (Utils::isInstanceOf(objA, typeid(StaticBody)) &&
		Utils::isInstanceOf(objB, typeid(DynamicBody))) {
		result.mode = StaticVsDyn;
	}

	return result;

}




ContactType Collision::FindContactPoints(GameObject* objA, GameObject* objB) {
	Vector2f contact1 = Vector2f(0, 0);
	Vector2f contact2 = Vector2f(0, 0);
	int contactsNumber = 0;
	std::vector <Vector2f> verticesA; 

	verticesA = objA->CalculateVertices();
	
	std::vector <Vector2f> verticesB;

	verticesB = objB->CalculateVertices();
	

	float minDistSq = INFINITY;
	

	for (int i = 0; i < verticesA.size(); i++) {
		Vector2f p = verticesA[i];
		for (int j = 0; j < verticesB.size(); j++) {
			Vector2f va = verticesB[j];
			Vector2f vb = verticesB[(j + 1) % verticesB.size()];

			PointToSegmentType p2s = PointToSegmentDistance(p, va, vb);

			

			if (Math::NearlyEqual(p2s.distSq, minDistSq)) {
				if (!Math::NearlyEqual(p2s.closestPoint, contact1) &&
					!Math::NearlyEqual(p2s.closestPoint, contact2)) {
					contact2 = p2s.closestPoint;

					contactsNumber = 2;
				}
			}

			else if (p2s.distSq < minDistSq) {
				minDistSq = p2s.distSq;

				contactsNumber = 1;

				contact1 = p2s.closestPoint;
			}
		}
	}
	/******************************************************/
	for (int i = 0; i < verticesB.size(); i++) {
		Vector2f p = verticesB[i];
		for (int j = 0; j < verticesA.size(); j++) {
			Vector2f va = verticesA[j];
			Vector2f vb = verticesA[(j + 1) % verticesA.size()];

			PointToSegmentType p2s = PointToSegmentDistance(p, va, vb);

			if (Math::NearlyEqual(p2s.distSq, minDistSq)) {
				if (!Math::NearlyEqual(p2s.closestPoint, contact1) && 
					!Math::NearlyEqual(p2s.closestPoint, contact2)) {
					contact2 = p2s.closestPoint;

					contactsNumber = 2;

					
				}
			}

			else if (p2s.distSq < minDistSq) {
				minDistSq = p2s.distSq;

				contactsNumber = 1;

				contact1 = p2s.closestPoint;
			}
		}
	}


	return ContactType(contact1, contact2, contactsNumber);
}



bool Collision::IntersectAABB(SDL_FRect AABBobjA, SDL_FRect AABBobjB) {
	return (AABBobjA.x < AABBobjB.x + AABBobjB.w &&
		AABBobjA.x + AABBobjA.w > AABBobjB.x &&
		AABBobjA.y < AABBobjA.y + AABBobjA.h &&
		AABBobjA.y + AABBobjA.h > AABBobjB.y);
}
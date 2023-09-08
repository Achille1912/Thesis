#pragma once

#include <vector>

#include "Vector2.h"

struct Math
{

    static float Dot(const Vector2f &a, const Vector2f &b);

    static float Cross(const Vector2f &a, const Vector2f &b);

    static float Rad(float deg);

    static float Deg(float rad);

    static float GetDistanceSquared(const Vector2f &t_vectorA, const Vector2f &t_vectorB);

    static Vector2f VectorRotation(const Vector2f &point, const Vector2f &center, float rotation_deg);

    static Vector2f FindArithmeticMean(const std::array<Vector2f, 4> &vertices);

    static bool NearlyEqual(float a, float b);

    static bool NearlyEqual(const Vector2f &a, const Vector2f &b);

    static bool AlmostEqual(float a, float b);

    static bool AlmostEqual(const Vector2f& a, const Vector2f& b);

    static float DistanceSquared(const Vector2f &a, const Vector2f &b);

    static Vector2f BasicResponseImpulse(float restitution, const Vector2f &vrel, const Vector2f &normal, float invMassA, float invMassB);

    static Vector2f CalculateNormal(Vector2f pointA, Vector2f pointB);

    static std::array<Vector2f, 4> CalculateNormals(std::array<Vector2f, 4>& vertices);

};

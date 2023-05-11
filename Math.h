#pragma once

#include "Vector2.h"
#include <vector>

struct Math {

    static float Dot(Vector2f a, Vector2f b);

    static float Cross(Vector2f a, Vector2f b);

    static float Rad(float deg);

    static float Deg(float rad);

    static float getDistanceSquared(Vector2f t_vectorA, Vector2f t_vectorB);

    static Vector2f VectorRotation(Vector2f point, Vector2f center, float rotation_deg);

    static Vector2f FindArithmeticMean(std::vector <Vector2f> vertices) ;

    static bool NearlyEqual(float a, float b) ;

    static bool NearlyEqual(Vector2f a, Vector2f b) ;

    static float DistanceSquared(Vector2f a, Vector2f b);

    static Vector2f BasicResponseImpulse(float restitution, Vector2f vrel, Vector2f normal, float invMassA, float invMassB);

};


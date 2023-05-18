#include "Math.h"
#include "Vector2.h"
#include "Constants.h"

float Math::Cross(Vector2f a, Vector2f b) {
    return (a.x * b.y - a.y * b.x);
}

float Math::Dot(Vector2f a, Vector2f b) {
    return(a.x * b.x + a.y * b.y);
}

float Math::Rad(float deg) {
    return(deg * PI / 180);
}

float Math::Deg(float rad) {
    return(rad * 180 / PI);
}

Vector2f Math::VectorRotation(Vector2f point, Vector2f center, float rotation_deg) {
    float angle = Rad(rotation_deg);

    Vector2f centered_point = Vector2f(point.x - center.x, point.y - center.y);

    Vector2f rot_centered_point = Vector2f(cos(angle) * centered_point.x - sin(angle) * centered_point.y, sin(angle) * centered_point.x + cos(angle) * centered_point.y);

    Vector2f result = rot_centered_point - center;

    return result;
}

float Math::getDistanceSquared(Vector2f t_vectorA, Vector2f t_vectorB) {
    float dx = t_vectorA.x - t_vectorB.x;
    float dy = t_vectorA.y - t_vectorB.y;
    return (dx * dx + dy * dy);
}

Vector2f Math::FindArithmeticMean(std::vector <Vector2f> vertices) {
    float sumX = 0;
    float sumY = 0;

    for (int i = 0; i < vertices.size(); i++) {
        Vector2f v = vertices[i];
        sumX += v.x;
        sumY += v.y;
    }
    return Vector2f(sumX / vertices.size(), sumY / vertices.size());
}


bool Math::NearlyEqual(float a, float b) {
    return std::abs(a - b) < SmallAmount;
}

bool Math::NearlyEqual(Vector2f a, Vector2f b) {
    return NearlyEqual(a.x, b.x) && NearlyEqual(a.y, b.y);
}

float Math::DistanceSquared(Vector2f a, Vector2f b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return dx * dx + dy * dy;
}

Vector2f Math::BasicResponseImpulse(float restitution, Vector2f vrel, Vector2f normal, float invMassA, float invMassB) {

    float contactVelocityMag = Math::Dot(vrel, normal);

    float denom =invMassA + invMassB;

    float j = -(1 + restitution) * contactVelocityMag;

    j /= denom;

   return (normal * j);

}


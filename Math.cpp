#include "Math.h"
#include "Vector2.h"
#include "Constants.h"
#include <array>

float Math::Cross(const Vector2f& a, const Vector2f& b) {
    return (a.x * b.y - a.y * b.x);
}

float Math::Dot(const Vector2f& a, const Vector2f& b) {
    return(a.x * b.x + a.y * b.y);
}

float Math::Rad(float deg) {
    return(deg * PI / 180);
}

float Math::Deg(float rad) {
    return(rad * 180 / PI);
}

Vector2f Math::VectorRotation(const Vector2f& point, const Vector2f& center, float rotation_deg) {
    float angle = Rad(rotation_deg);
    float cos_angle = cos(angle);
    float sin_angle = sin(angle);

    Vector2f centered_point(point.x - center.x, point.y - center.y);

    Vector2f rot_centered_point(cos_angle * centered_point.x - sin_angle * centered_point.y, sin_angle * centered_point.x + cos_angle * centered_point.y);

    return (rot_centered_point - center);

}

float Math::GetDistanceSquared(const Vector2f& t_vectorA, const Vector2f& t_vectorB) {
    float dx = t_vectorA.x - t_vectorB.x;
    float dy = t_vectorA.y - t_vectorB.y;
    return (dx * dx + dy * dy);
}

Vector2f Math::FindArithmeticMean(const std::array <Vector2f, 4>& vertices) {
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

bool Math::NearlyEqual(const Vector2f& a, const Vector2f& b) {
    return NearlyEqual(a.x, b.x) && NearlyEqual(a.y, b.y);
}

float Math::DistanceSquared(const Vector2f& a, const Vector2f& b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return dx * dx + dy * dy;
}

Vector2f Math::BasicResponseImpulse(float restitution, const Vector2f& vrel, const Vector2f& normal, float invMassA, float invMassB) {

    float contactVelocityMag = Math::Dot(vrel, normal);

    float denom =invMassA + invMassB;

    float j = -(1 + restitution) * contactVelocityMag;

    j /= denom;
    Vector2f normal_result = normal;
   return (normal_result * j);

}


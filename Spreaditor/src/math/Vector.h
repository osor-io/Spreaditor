#pragma once

#include <SFML/System/Vector2.hpp>
#include <cmath>


using Vector2f = sf::Vector2f;

inline float dot(const Vector2f& a, const Vector2f& b) {
    return ((a.x*b.x) + (a.y*b.y));
}

inline float cross(const Vector2f& a, const Vector2f& b) {
    return (a.x*b.y) - (a.y*b.x);
}

inline float magnitude(const Vector2f& v) {
    return std::sqrtf(dot(v, v));
}

inline float magnitude_squared(const Vector2f& v) {
    return (dot(v, v));
}

inline Vector2f normalize(const Vector2f& v) {
    auto mag = magnitude(v);
    if (mag != 0.0f) {
        return Vector2f(v.x / mag, v.y / mag);
    }
    return Vector2f(0.0f, 0.0f);
}

inline Vector2f tangent(const Vector2f& v) {
    return Vector2f(-v.y , v.x);
}

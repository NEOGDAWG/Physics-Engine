#pragma once
#include <cmath>

struct Vec2 {
    float x, y;

    Vec2() : x(0), y(0) {}
    Vec2(float x, float y) : x(x), y(y) {}

    Vec2 operator + (const Vec2& rhs) const { return Vec2(x + rhs.x, y + rhs.y); }
    Vec2 operator - (const Vec2& rhs) const { return Vec2(x - rhs.x, y - rhs.y); }
    Vec2 operator * (float scalar)    const { return Vec2(x * scalar, y * scalar); }
    Vec2 operator / (float scalar)    const { return Vec2(x / scalar, y / scalar); }

    Vec2& operator += (const Vec2& rhs) { x += rhs.x; y += rhs.y; return *this; }
    Vec2& operator -= (const Vec2& rhs) { x -= rhs.x; y -= rhs.y; return *this; }

    Vec2 operator-() const { return Vec2(-x, -y); }

    float LengthSquared() const { return x * x + y * y; }
    float Dot(const Vec2& other) const { return x * other.x + y * other.y; }
};

// Length (magnitude) of a vector
inline float Length(const Vec2& v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

// Normalize a vector (make length = 1)
inline Vec2 Normalize(const Vec2& v) {
    float len = Length(v);
    return len > 0 ? v / len : Vec2(0, 0);
}

// Dot product (used in impulse resolution)
inline float Dot(const Vec2& a, const Vec2& b) {
    return a.x * b.x + a.y * b.y;
}

inline Vec2 min(const Vec2& a, const Vec2& b) {
    if(a.x * a.y < b.x * b.y){
        return a;
    }
    else{
        return b;
    }
}

// Clamp a value between min and max
inline float Clamp(float val, float min, float max) {
    return (val < min) ? min : (val > max) ? max : val;
}
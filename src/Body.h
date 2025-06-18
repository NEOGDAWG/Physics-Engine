#pragma once
#include "Math.h"
#include "Shape.h"

class Body {
public:
    Vec2 position;
    Vec2 velocity;
    float mass;
    float restitution;
    bool isStatic;
    Shape* shape;  // Pointer to the shape

    // Constructor for circle
    Body(Vec2 pos, float radius, float m, float rest, bool stat = false);
    // Constructor for AABB
    Body(Vec2 pos, Vec2 size, float m, float rest, bool stat = false);
    ~Body();

    void ApplyImpulse(Vec2 impulse);
    void Update(float dt);
};


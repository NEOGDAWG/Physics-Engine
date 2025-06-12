#pragma once
#include "Math.h"

class Body {
public:
    Vec2 position;
    Vec2 velocity;
    float radius;
    float mass;
    float restitution;  // Bounciness (0 = no bounce, 1 = perfect)
    bool isStatic;

    // Constructor
    Body(Vec2 pos, float r, float m, float rest, bool stat = false);

    // Apply a force-like impulse to the velocity
    void ApplyImpulse(const Vec2& impulse);

    // Update position based on current velocity
    void Update(float dt);
};

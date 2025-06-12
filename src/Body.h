#pragma once
#include "Math.h"

class Body {
public:
    Vec2 position;
    Vec2 velocity;
    float radius;
    float mass;
    float restitution;
    bool isStatic;

    Body(Vec2 pos, float r, float m, float rest, bool stat = false);
    void ApplyImpulse(Vec2 impulse);
    void Update(float dt);
};


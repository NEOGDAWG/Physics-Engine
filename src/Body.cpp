#include "Body.h"

Body::Body(Vec2 pos, float r, float m, float rest, bool stat)
    : position(pos), radius(r), mass(m), restitution(rest), isStatic(stat), velocity(Vec2(0, 0)) {}

void Body::ApplyImpulse(const Vec2& impulse) {
    if (isStatic) return;
    velocity += impulse / mass;
}

void Body::Update(float dt) {
    if (isStatic) return;
    position += velocity * dt;
}

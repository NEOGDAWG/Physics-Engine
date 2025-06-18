#include "Body.h"

Body::Body(Vec2 pos, float radius, float m, float rest, bool stat) 
    : position(pos), velocity(Vec2(0, 0)), mass(m), restitution(rest), isStatic(stat) {
    shape = new Circle(radius);
}

Body::Body(Vec2 pos, Vec2 size, float m, float rest, bool stat)
    : position(pos), velocity(Vec2(0, 0)), mass(m), restitution(rest), isStatic(stat) {
    shape = new AABB(size);
}

Body::~Body() {
    delete shape;
}

void Body::ApplyImpulse(Vec2 impulse) {
    if (isStatic) return;
    velocity = velocity + impulse * (1.0f / mass);
}

void Body::Update(float dt) {
    if (isStatic) return;
    position = position + velocity * dt;
    shape->UpdatePosition(position);
}


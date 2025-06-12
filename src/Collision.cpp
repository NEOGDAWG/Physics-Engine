#include "Collision.h"

void CheckAndResolveCollision(Body* a, Body* b) {
    if (a->isStatic && b->isStatic) return;

    Vec2 difference = b->position - a->position;
    float distance = Length(difference);
    float minDistance = a->radius + b->radius;

    if (distance >= minDistance || distance == 0.0f) {
        return; // No collision
    }

    // Normalize direction
    Vec2 normal = Normalize(difference);

    // Penetration resolution
    float penetration = minDistance - distance;
    Vec2 correction = normal * (penetration / (a->isStatic || b->isStatic ? 1.0f : 2.0f));

    if (!a->isStatic) {
        a->position -= correction;
    }
    if (!b->isStatic) {
        b->position += correction;
    }

    // Relative velocity
    Vec2 relativeVelocity = b->velocity - a->velocity;
    float velAlongNormal = Dot(relativeVelocity, normal);

    if (velAlongNormal > 0) {
        return; // Already separating
    }

    // Restitution (bounciness)
    float e = std::min(a->restitution, b->restitution);

    // Calculate impulse scalar
    float invMassA = a->isStatic ? 0.0f : 1.0f / a->mass;
    float invMassB = b->isStatic ? 0.0f : 1.0f / b->mass;

    float j = -(1 + e) * velAlongNormal;
    j /= (invMassA + invMassB);

    Vec2 impulse = normal * j;

    if (!a->isStatic) {
        a->ApplyImpulse(-impulse);
    }
    if (!b->isStatic) {
        b->ApplyImpulse(impulse);
    }
}

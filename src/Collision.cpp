#include <algorithm>
#include "Collision.h"
#include <iostream>

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

    // Debug: Print normal
    std::cout << "Normal: (" << normal.x << ", " << normal.y << ")\n";

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

    // Debug: Print velAlongNormal
    std::cout << "velAlongNormal: " << velAlongNormal << "\n";

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

    // Debug: Print impulse
    std::cout << "Impulse: (" << impulse.x << ", " << impulse.y << ")\n";

    // Debug: Print velocities before impulse
    std::cout << "Before impulse: a->velocity = (" << a->velocity.x << ", " << a->velocity.y << ")\n";
    std::cout << "Before impulse: b->velocity = (" << b->velocity.x << ", " << b->velocity.y << ")\n";

    if (!a->isStatic) {
        a->ApplyImpulse(-impulse);
    }
    if (!b->isStatic) {
        b->ApplyImpulse(impulse);
    }

    // Debug: Print velocities after impulse
    std::cout << "After impulse: a->velocity = (" << a->velocity.x << ", " << a->velocity.y << ")\n";
    std::cout << "After impulse: b->velocity = (" << b->velocity.x << ", " << b->velocity.y << ")\n";
}

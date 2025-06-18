#include <algorithm>
#include "Collision.h"
#include <iostream>
#include <cmath>

bool CheckCircleCircleCollision(const Circle* a, const Vec2& posA,
                              const Circle* b, const Vec2& posB,
                              Contact& contact) {
    Vec2 diff = posB - posA;
    float distanceSquared = diff.LengthSquared();
    float radiusSum = a->radius + b->radius;
    
    if (distanceSquared > radiusSum * radiusSum) {
        return false;
    }

    float distance = std::sqrt(distanceSquared);
    contact.normal = diff * (1.0f / distance);
    contact.penetration = radiusSum - distance;
    contact.contactPoint = posA + contact.normal * a->radius;
    
    return true;
}

bool CheckAABBAABBCollision(const AABB* a, const Vec2& posA,
                           const AABB* b, const Vec2& posB,
                           Contact& contact) {
    Vec2 diff = posB - posA;
    Vec2 overlap(
        a->halfSize.x + b->halfSize.x - std::abs(diff.x),
        a->halfSize.y + b->halfSize.y - std::abs(diff.y)
    );

    if (overlap.x <= 0 || overlap.y <= 0) {
        return false;
    }

    // Find the axis of minimum penetration
    if (overlap.x < overlap.y) {
        contact.normal = Vec2(diff.x > 0 ? 1.0f : -1.0f, 0.0f);
        contact.penetration = overlap.x;
    } else {
        contact.normal = Vec2(0.0f, diff.y > 0 ? 1.0f : -1.0f);
        contact.penetration = overlap.y;
    }

    contact.contactPoint = posA + contact.normal * contact.normal.Dot(a->halfSize);
    return true;
}

bool CheckCircleAABBCollision(const Circle* circle, const Vec2& circlePos,
                             const AABB* box, const Vec2& boxPos,
                             Contact& contact) {
    // Find closest point on AABB to circle center
    Vec2 closest(
        std::max(boxPos.x - box->halfSize.x, std::min(circlePos.x, boxPos.x + box->halfSize.x)),
        std::max(boxPos.y - box->halfSize.y, std::min(circlePos.y, boxPos.y + box->halfSize.y))
    );

    Vec2 diff = circlePos - closest;
    float distanceSquared = diff.LengthSquared();

    if (distanceSquared > circle->radius * circle->radius) {
        return false;
    }

    float distance = std::sqrt(distanceSquared);
    contact.normal = diff * (1.0f / distance);
    contact.penetration = circle->radius - distance;
    contact.contactPoint = closest;
    
    return true;
}

void ResolveCollision(Body* a, Body* b, const Contact& contact) {
    // Calculate relative velocity
    Vec2 relativeVel = b->velocity - a->velocity;
    
    // Calculate relative velocity in terms of the normal direction
    float velAlongNormal = relativeVel.Dot(contact.normal);
    
    // Do not resolve if objects are moving apart
    if (velAlongNormal > 0) {
        return;
    }

    // Calculate restitution
    float restitution = std::min(a->restitution, b->restitution);

    // Calculate impulse scalar
    float j = -(1.0f + restitution) * velAlongNormal;
    j /= a->isStatic ? 0 : 1.0f/a->mass;
    j /= b->isStatic ? 0 : 1.0f/b->mass;

    // Apply impulse
    Vec2 impulse = contact.normal * j;
    if (!a->isStatic) a->ApplyImpulse(-impulse);
    if (!b->isStatic) b->ApplyImpulse(impulse);

    // Positional correction to prevent sinking
    const float percent = 0.2f; // penetration recovery percent
    const float slop = 0.01f;   // penetration allowance
    Vec2 correction = contact.normal * (std::max(contact.penetration - slop, 0.0f) / 
        (a->isStatic ? 0 : 1.0f/a->mass + (b->isStatic ? 0 : 1.0f/b->mass))) * percent;
    
    if (!a->isStatic) a->position = a->position - correction * (1.0f/a->mass);
    if (!b->isStatic) b->position = b->position + correction * (1.0f/b->mass);
}

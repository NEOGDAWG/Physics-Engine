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
    float distance = std::sqrt(distanceSquared);

    // Check if circle edge touches AABB edge
    if (distance > circle->radius) {
        return false;
    }

    if (distance == 0.0f) {
        // Circle center is exactly on the AABB, choose a default normal
        contact.normal = Vec2(0, 1);
        contact.penetration = circle->radius;
        contact.contactPoint = circlePos - contact.normal * circle->radius;
        return true;
    }
    
    // Normal points from AABB toward circle center
    contact.normal = diff * (1.0f / distance);
    contact.penetration = circle->radius - distance;
    contact.contactPoint = circlePos - contact.normal * circle->radius; // Contact point on circle's edge
    
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
    float invMassA = a->isStatic ? 0.0f : 1.0f/a->mass;
    float invMassB = b->isStatic ? 0.0f : 1.0f/b->mass;
    float invMassSum = invMassA + invMassB;
    if (invMassSum == 0.0f) return; // Prevent division by zero
    j /= invMassSum;

    // Apply impulse
    Vec2 impulse = contact.normal * j;
    if (!a->isStatic) a->ApplyImpulse(-impulse);
    if (!b->isStatic) b->ApplyImpulse(impulse);

    // Debug print: before correction
    std::cout << "Before correction:\n";
    std::cout << "  a->position: (" << a->position.x << ", " << a->position.y << ")\n";
    std::cout << "  b->position: (" << b->position.x << ", " << b->position.y << ")\n";
    std::cout << "  contact.normal: (" << contact.normal.x << ", " << contact.normal.y << ")\n";
    std::cout << "  contact.penetration: " << contact.penetration << "\n";

    // Simple position correction: move out by penetration
    if (contact.penetration > 0.0f) {
        Vec2 correction;
        if (!a->isStatic && b->isStatic) {
            correction = contact.normal * contact.penetration;
            a->position = a->position - correction;
            // Clamp: ensure the ball's bottom never goes below the floor's top
            if (a->shape->GetType() == Shape::Type::Circle && b->shape->GetType() == Shape::Type::AABB) {
                float ballRadius = static_cast<Circle*>(a->shape)->radius;
                float floorTop = b->position.y - static_cast<AABB*>(b->shape)->halfSize.y;
                float ballBottom = a->position.y + ballRadius;
                if (ballBottom > floorTop) {
                    a->position.y = floorTop - ballRadius;
                }
            }
        } else if (!b->isStatic && a->isStatic) {
            correction = contact.normal * contact.penetration;
            b->position = b->position + correction;
        } else if (!a->isStatic && !b->isStatic) {
            correction = contact.normal * (contact.penetration * 0.5f);
            a->position = a->position - correction;
            b->position = b->position + correction;
        }
        // Debug print: applied correction
        std::cout << "  Applied correction: (" << correction.x << ", " << correction.y << ")\n";
    }
    // Debug print: after correction
    std::cout << "After correction:\n";
    std::cout << "  a->position: (" << a->position.x << ", " << a->position.y << ")\n";
    std::cout << "  b->position: (" << b->position.x << ", " << b->position.y << ")\n";
}

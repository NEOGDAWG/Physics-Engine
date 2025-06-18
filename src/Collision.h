#pragma once
#include "Body.h"

struct Contact {
    Vec2 normal;
    float penetration;
    Vec2 contactPoint;
};

bool CheckCircleCircleCollision(const Circle* a, const Vec2& posA, 
                              const Circle* b, const Vec2& posB,
                              Contact& contact);

bool CheckAABBAABBCollision(const AABB* a, const Vec2& posA,
                           const AABB* b, const Vec2& posB,
                           Contact& contact);

bool CheckCircleAABBCollision(const Circle* circle, const Vec2& circlePos,
                             const AABB* box, const Vec2& boxPos,
                             Contact& contact);

void ResolveCollision(Body* a, Body* b, const Contact& contact);
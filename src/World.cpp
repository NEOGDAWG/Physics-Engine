#include "World.h"

World::World(const Vec2& gravity) : gravity(gravity) {}

World::~World() {
    for (Body* body : bodies) {
        delete body;
    }
}

void World::AddBody(Body* body) {
    bodies.push_back(body);
}

void World::Update(float dt) {
    // Apply gravity and update positions
    for (Body* body : bodies) {
        if (!body->isStatic) {
            body->velocity = body->velocity + gravity * dt;
        }
        body->Update(dt);
    }

    // Check and resolve collisions
    CheckCollisions();
}

void World::CheckCollisions() {
    for (size_t i = 0; i < bodies.size(); ++i) {
        for (size_t j = i + 1; j < bodies.size(); ++j) {
            CheckAndResolveCollision(bodies[i], bodies[j]);
        }
    }
}

void World::CheckAndResolveCollision(Body* a, Body* b) {
    Contact contact;
    bool collision = false;

    // Get shape types
    Shape::Type typeA = a->shape->GetType();
    Shape::Type typeB = b->shape->GetType();

    // Check collision based on shape types
    if (typeA == Shape::Type::Circle && typeB == Shape::Type::Circle) {
        collision = CheckCircleCircleCollision(
            static_cast<Circle*>(a->shape), a->position,
            static_cast<Circle*>(b->shape), b->position,
            contact
        );
    }
    else if (typeA == Shape::Type::AABB && typeB == Shape::Type::AABB) {
        collision = CheckAABBAABBCollision(
            static_cast<AABB*>(a->shape), a->position,
            static_cast<AABB*>(b->shape), b->position,
            contact
        );
    }
    else if (typeA == Shape::Type::Circle && typeB == Shape::Type::AABB) {
        collision = CheckCircleAABBCollision(
            static_cast<Circle*>(a->shape), a->position,
            static_cast<AABB*>(b->shape), b->position,
            contact
        );
    }
    else if (typeA == Shape::Type::AABB && typeB == Shape::Type::Circle) {
        collision = CheckCircleAABBCollision(
            static_cast<Circle*>(b->shape), b->position,
            static_cast<AABB*>(a->shape), a->position,
            contact
        );
        if (collision) {
            contact.normal = -contact.normal;
        }
    }

    if (collision) {
        ResolveCollision(a, b, contact);
    }
}

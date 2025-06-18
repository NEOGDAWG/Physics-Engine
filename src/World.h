#pragma once
#include <vector>
#include "Body.h"
#include "Collision.h"

class World {
public:
    World(const Vec2& gravity);
    ~World();

    void AddBody(Body* body);
    void Update(float dt);

private:
    Vec2 gravity;
    std::vector<Body*> bodies;

    void CheckCollisions();
    void CheckAndResolveCollision(Body* a, Body* b);
};

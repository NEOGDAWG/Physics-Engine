#pragma once
#include <vector>
#include "Body.h"
#include "Collision.h"
#include <SFML/Graphics.hpp>


class World {
public:
    World(const Vec2& gravity);
    ~World();

    void AddBody(Body* body);
    void Update(float dt);
    const std::vector<Body*>& GetBodies() const;
    void DrawBody(sf::RenderWindow& window, Body* body, sf::Color color);
    void removeAllBodies();

private:
    Vec2 gravity;
    std::vector<Body*> bodies;

    void CheckCollisions();
    void CheckAndResolveCollision(Body* a, Body* b);
};

#include <iostream>
#include "src/Math.h"
#include "src/Body.h"
#include "src/Collision.h"
#include <algorithm>

int main() {
    // Light ball (mass = 1)
    Body light(Vec2(0, 0), 1.0f, 1.0f, 0.8f, false);
    light.velocity = Vec2(5, 0); // Moving right

    // Heavy ball (mass = 5)
    Body heavy(Vec2(1.5, 0), 1.0f, 5.0f, 0.8f, false);
    heavy.velocity = Vec2(-1, 0); // Moving left

    std::cout << "Before Collision:\n";
    std::cout << "Light velocity: " << light.velocity.x << ", " << light.velocity.y << "\n";
    std::cout << "Heavy velocity: " << heavy.velocity.x << ", " << heavy.velocity.y << "\n";

    // Run collision resolution
    CheckAndResolveCollision(&light, &heavy);

    std::cout << "\nAfter Collision:\n";
    std::cout << "Light velocity: " << light.velocity.x << ", " << light.velocity.y << "\n";
    std::cout << "Heavy velocity: " << heavy.velocity.x << ", " << heavy.velocity.y << "\n";

    return 0;
}



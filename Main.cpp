#include <iostream>
#include "src/Math.h"
#include "src/Body.h"
#include "src/Collision.h"
#include "src/World.h"
#include <SFML/Graphics.hpp>

int main() {
    World world(Vec2(0, -9.8f)); // gravity down

    // Create floor (AABB)
    Body* floor = new Body(Vec2(0, -5), Vec2(10, 1), 1.0f, 0.6f, true);
    
    // Create a circle
    Body* ball = new Body(Vec2(0, 5), 1.0f, 1.0f, 0.6f, false);
    
    // Create a box
    Body* box = new Body(Vec2(2, 3), Vec2(1, 1), 1.0f, 0.6f, false);

    world.AddBody(floor);
    world.AddBody(ball);
    world.AddBody(box);

    for (int i = 0; i < 120; ++i) {
        world.Update(0.016f); // 60 fps
        std::cout << "t=" << i << ":\n"
                  << "Ball pos = (" << ball->position.x << ", " << ball->position.y << ")\n"
                  << "Box pos = (" << box->position.x << ", " << box->position.y << ")\n"
                  << "-------------------\n";
    }

    return 0;
}

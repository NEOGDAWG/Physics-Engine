#include <iostream>
#include "src/Math.h"
#include "src/Body.h"
#include "src/Collision.h"
#include "src/World.h"
#include <SFML/Graphics.hpp>

int main()
{
    unsigned int width = 1300;
    unsigned int height = 800;
    sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode({width, height}), "Physics Engine");


    World* world = new World(Vec2(0, 1.5));
    Body* body = new Body(Vec2(650, 200), 50, 10, 0.6, false);
    Body* floor = new Body(Vec2(650, 775), Vec2(1300, 50), 0, 0.9, true);
    world->AddBody(body);
    world->AddBody(floor);

    float dt = 1.0f / 140.0f;
    while (window->isOpen()){
        while(auto event = window->pollEvent()){
            if(event->is<sf::Event::Closed>()){
                window->close();
            }
            else if(const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()){
                if(keyEvent->scancode == sf::Keyboard::Scancode::Escape){
                    window->close();
                }
            }
        }
        world->Update(dt);
        window->clear();
        
        // Draw the ball
        sf::CircleShape ball(50.f); // radius of 50
        ball.setOrigin(sf::Vector2f(50.f, 50.f)); // set origin to center
        ball.setPosition(sf::Vector2f(body->position.x, body->position.y)); // draw at center
        ball.setFillColor(sf::Color::Red);
        window->draw(ball);
        
        // Draw the floor
        sf::RectangleShape ground({1300, 50});
        ground.setPosition({floor->position.x - 1300/2, floor->position.y - 50/2}); // center the rectangle
        ground.setFillColor(sf::Color::Green);
        window->draw(ground);
        
        window->display();
    }
    //comment

    return 0;
}

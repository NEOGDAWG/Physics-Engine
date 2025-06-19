#include <iostream>
#include "src/Math.h"
#include "src/Body.h"
#include "src/Collision.h"
#include "src/World.h"
#include <SFML/Graphics.hpp>

int main()
{
    unsigned int width = 650;
    unsigned int height = 380;
    sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode({width, height}), "Physics Engine");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
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
        window->clear();
        window->draw(shape);
        window->display();
    }

    return 0;
}

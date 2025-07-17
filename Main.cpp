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

    enum class SpawnShape { Circle, AABB};
    SpawnShape currentShape = SpawnShape::Circle;
    float circleRadius = 30.0f;
    Vec2 AABBSize(60.0f, 60.0f);
    float mass = 1.0f;
    float restitution = 0.6f;
    bool isStatic = false;
    const float minRadius = 5.0f;
    const float maxRadius = 200.0f;
    const Vec2 minAABBSize = {5.0f, 5.0f};
    const Vec2 maxAABBSize = {400.0f, 400.0f};
    


    World* world = new World(Vec2(0, 1.5));
    Body* floor = new Body(Vec2(650, 775), Vec2(1300, 50), 0, 0.9, true);
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
                else if(keyEvent->scancode == sf::Keyboard::Scancode::C){
                    currentShape = SpawnShape::Circle;
                }
                else if(keyEvent->scancode == sf::Keyboard::Scancode::B){
                    currentShape = SpawnShape::AABB;
                }
                else if(keyEvent->scancode == sf::Keyboard::Scancode::Up){
                    if(currentShape == SpawnShape::Circle){
                        circleRadius = std::min(maxRadius, circleRadius + 5.0f);
                    }
                    else if(currentShape == SpawnShape::AABB){
                        AABBSize.x = std::min(maxAABBSize.x, AABBSize.x + 5.0f);
                        AABBSize.y = std::min(maxAABBSize.y, AABBSize.y + 5.0f);
                    }
                }
                else if(keyEvent->scancode == sf::Keyboard::Scancode::Down){
                    if(currentShape == SpawnShape::Circle){
                        circleRadius = std::max(minRadius, circleRadius - 5.0f);
                    }
                    else if(currentShape == SpawnShape::AABB){
                        AABBSize.x = std::max(minAABBSize.x, AABBSize.x - 5.0f);
                        AABBSize.y = std::max(minAABBSize.y, AABBSize.y - 5.0f);
                    }
                }
                else if(keyEvent->scancode == sf::Keyboard::Scancode::Space){
                    world->removeAllBodies();
                    Body* floor = new Body(Vec2(650, 775), Vec2(1300, 50), 0, 0.9, true);
                    world->AddBody(floor);
                }

            }
            else if(const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()){
                if(mouseEvent->button == sf::Mouse::Button::Left){
                    sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
                    Vec2 pos(mousePos.x, mousePos.y);
                    if(currentShape == SpawnShape::Circle){
                        world->AddBody(new Body(pos, circleRadius, mass, restitution, false));
                    }
                    else if(currentShape == SpawnShape::AABB){
                        world->AddBody(new Body(pos, AABBSize, mass, 0.0f, false));
                    }
                }
            }
        }
        world->Update(dt);
        window->clear();

        for(Body* body : world->GetBodies()){
            sf::Color color = body->isStatic ? sf::Color::Blue : sf::Color::Red;
            world->DrawBody(*window, body, color);
        }

        sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
        if (currentShape == SpawnShape::Circle) {
            sf::CircleShape preview(circleRadius);
            preview.setOrigin(sf::Vector2f(circleRadius, circleRadius));
            preview.setPosition(sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)));
            preview.setFillColor(sf::Color(255, 0, 0, 100)); // Red, semi-transparent
            window->draw(preview);
        } else if (currentShape == SpawnShape::AABB) {
            sf::RectangleShape preview(sf::Vector2f(AABBSize.x, AABBSize.y));
            preview.setOrigin(sf::Vector2f(AABBSize.x / 2, AABBSize.y / 2));
            preview.setPosition(sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)));
            preview.setFillColor(sf::Color(0, 0, 255, 100)); // Blue, semi-transparent
            window->draw(preview);
        }
        window->display();
    }
    //comment

    return 0;
}

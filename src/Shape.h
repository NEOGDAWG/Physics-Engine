#pragma once
#include "Math.h"

// Base shape class
class Shape {
public:
    enum class Type {
        Circle,
        AABB
    };

    virtual ~Shape() = default;
    virtual Type GetType() const = 0;
    virtual void UpdatePosition(const Vec2& position) = 0;
};

class Circle : public Shape {
public:
    float radius;

    Circle(float r) : radius(r) {}
    
    Type GetType() const override { return Type::Circle; }
    
    void UpdatePosition(const Vec2& position) override {
       
    }
};


class AABB : public Shape {
public:
    Vec2 halfSize;  

    AABB(const Vec2& size) : halfSize(size * 0.5f) {}
    
    Type GetType() const override { return Type::AABB; }
    
    void UpdatePosition(const Vec2& position) override {

    }
}; 
#include <iostream>
#include "src/Math.h"

int main() {
    Vec2 a(3, 4);
    Vec2 b(1, 2);
    
    Vec2 result = a + b;
    std::cout << "a + b = (" << result.x << ", " << result.y << ")\n";
    std::cout << "Dot: " << Dot(a, b) << "\n";
    std::cout << "Length of a: " << Length(a) << "\n";

    return 0;
}

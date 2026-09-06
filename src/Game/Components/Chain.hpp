#pragma once
#include <vector>
#include <box2d/math_functions.h>

struct Chain {
    int pointCount;
    std::vector<Vector2> points;
};

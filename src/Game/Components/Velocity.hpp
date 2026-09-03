#pragma once
#include "raylib.h"

struct Velocity {
    Vector2 value = {0, 0};
    float   max   = INFINITY;
};

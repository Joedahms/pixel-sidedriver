#pragma once
#include <raylib.h>

struct Transform2D {
    Vector2 position                = {.x = 0, .y = 0};
    Vector2 positionOffset          = {.x = 0, .y = 0};
    float   rotationDegrees         = 0;
    float   previousRotationDegrees = 0;
    float   rotationOffsetDegrees   = 0;
};

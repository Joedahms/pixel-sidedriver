#pragma once
#include <raylib.h>

namespace Constants {
    constexpr Vector2 defaultWindowSize      = {.x = 1600, .y = 1280};
    constexpr int     fps                    = 60;
    constexpr float   pixelsPerMeter         = 50.0f;
    constexpr b2Vec2  playerStartingPosition = {.x = 0, .y = -1000};
}

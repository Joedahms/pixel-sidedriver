#pragma once
#include <raylib.h>

/**
 * Path is needed for loading sprites
 */
struct Sprite {
    Texture2D   texture;
    std::string path;
    Vector2     size = {.x = 0, .y = 0};

    int numFrames    = 0;
    int currentFrame = 0;
};

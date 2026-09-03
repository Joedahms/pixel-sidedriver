#pragma once
#include <chrono>

struct GameTime {
    float                        simulationSpeed = 1;
    float                        frameTime       = 0;
    std::chrono::duration<float> gameTime        = std::chrono::duration<float>(0);
    std::string                  formattedGameTime;
};

#pragma once
#include <chrono>

#include "GameState.hpp"

class Game {
public:
    explicit Game();

    auto run() -> GameStatus;

private:
    GameState gameState;

    const float initialZoom = 0.5f;

    void updateTime();

    void update();

    void draw();

    void setupCamera();

    void setupNewGame();
};
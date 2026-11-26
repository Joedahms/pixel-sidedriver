#pragma once

#include <box2d/box2d.h>

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

    static void setupPlayer(entt::registry& registry, b2WorldId worldId);
};
#pragma once

struct GameState;

struct Menu {
    virtual ~Menu() = default;
    virtual void draw(GameState &gameState) = 0;
};

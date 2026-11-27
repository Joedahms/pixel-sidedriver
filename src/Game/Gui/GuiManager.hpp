#pragma once

#include "../GameState.hpp"
#include "Menu/PauseMenu/PauseMenu.hpp"

class GuiManager {
public:
    explicit GuiManager(entt::registry &registry);

    static void draw(GameState &gameState);

private:
    PauseMenu pauseMenu;
};

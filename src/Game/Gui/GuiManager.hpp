#pragma once

#include "../GameState.hpp"
#include "Menu/PauseMenu/PauseMenu.hpp"

class GuiManager {
public:
    explicit GuiManager(entt::registry &registry);

    void draw(GameState &gameState);

private:
    PauseMenu pauseMenu;

    entt::sigh<void(entt::registry &registry)> enterShipEditorSignal;
};

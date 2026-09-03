#pragma once

#include "PauseMenuState.hpp"
#include "SettingsMenu.hpp"
#include "../../../GameState.hpp"

class PauseMenu {
public:
    auto draw(GameState &gameState) -> void;

private:
    PauseMenuState pauseMenuState = PauseMenuState::None;
    SettingsMenu   settingsMenu;

    void checkPauseMenuState(GameState &gameState);
};

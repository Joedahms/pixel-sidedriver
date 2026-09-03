#pragma once

#include "InputBindingsMenu.hpp"
#include "PauseMenuState.hpp"
#include "SettingsMenuState.hpp"
#include "../../../GameState.hpp"

class SettingsMenu {
public:
   auto update(GameState &gameState) -> std::optional<PauseMenuState>;

private:
   SettingsMenuState settingsMenuState = SettingsMenuState::None;
   InputBindingsMenu inputBindingsMenu;

   auto checkSettingsMenuState(GameState &gameState) -> std::optional<PauseMenuState>;
};


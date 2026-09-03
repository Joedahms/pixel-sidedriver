#pragma once
#include "InputBindingsMenuState.hpp"
#include "SettingsMenuState.hpp"
#include "../../../GameState.hpp"

class InputBindingsMenu {
public:
    auto draw(GameState &gameState) -> std::optional<SettingsMenuState>;

private:
    InputBindingsMenuState inputBindingsMenuState = InputBindingsMenuState::None;

    auto checkInputBindingsMenuState() -> std::optional<SettingsMenuState>;
};

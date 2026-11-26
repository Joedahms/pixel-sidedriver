#include "GuiManager.hpp"

#include "GasGauge.hpp"
#include "Menu/PauseMenu/PauseMenu.hpp"

GuiManager::GuiManager(entt::registry &) {}

void GuiManager::draw(GameState &gameState) {
    auto &registry = gameState.registry;
    switch (gameState.gameplayState) {
        case GameplayState::Normal: {
            GasGauge::draw(registry);
            break;
        }
        case GameplayState::Paused: {
            pauseMenu.draw(gameState);
            break;
        }
    }
}

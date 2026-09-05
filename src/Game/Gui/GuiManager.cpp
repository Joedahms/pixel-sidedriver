#include "GuiManager.hpp"


#include "../Systems/ClickInfo/ClickInfoSystem.hpp"
#include "Menu/PauseMenu/PauseMenu.hpp"

GuiManager::GuiManager(entt::registry &registry) : hotbarGui(registry) {
}

void GuiManager::draw(GameState &gameState) {
    auto &registry = gameState.registry;
    switch (gameState.gameplayState) {
        case GameplayState::Normal: {
            hotbarGui.draw(registry);
            SimulationTime::draw(registry);
            ClickInfoSystem::update(registry);
            break;
        }
        case GameplayState::Paused: {
            pauseMenu.draw(gameState);
            break;
        }
    }
}
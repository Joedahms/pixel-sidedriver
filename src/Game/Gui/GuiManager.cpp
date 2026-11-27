#include "GuiManager.hpp"

#include "GasGauge.hpp"
#include "Menu/PauseMenu/PauseMenu.hpp"

GuiManager::GuiManager(entt::registry &) {}

void GuiManager::draw(GameState &gameState) {
    std::stack<std::unique_ptr<Menu> > &menuStack = gameState.menuStack;
    if (menuStack.empty()) {
        GasGauge::draw(gameState.registry);
        return;
    }
    menuStack.top()->draw(gameState);
}

#pragma once

#include "../GameState.hpp"
#include "Inventory/HotbarGui.hpp"
#include "Menu/PauseMenu/PauseMenu.hpp"
#include "ShipEditor/ShipEditor.hpp"
#include "SimulationTime/SimulationTime.hpp"

class GuiManager {
public:
    explicit GuiManager(entt::registry &registry);

    void draw(GameState &gameState);

private:
    PauseMenu      pauseMenu;
    SimulationTime simulationTime;
    HotbarGui      hotbarGui;

    entt::sigh<void(entt::registry &registry)> enterShipEditorSignal;

    void drawShipEditorButton(GameState &gameState) const;
};

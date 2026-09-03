#include "GuiManager.hpp"

#include <imgui.h>

#include "../Entities/Ship/Parts/Engine.hpp"
#include "../Systems/ClickInfo/ClickInfoSystem.hpp"
#include "Menu/PauseMenu/PauseMenu.hpp"

GuiManager::GuiManager(entt::registry &registry) : hotbarGui(registry) {
    entt::sink enterShipEditorSink{enterShipEditorSignal};
    enterShipEditorSink.connect<&ShipEditor::enter>(registry);
}

void GuiManager::draw(GameState &gameState) {
    auto &registry = gameState.registry;
    switch (gameState.gameplayState) {
        case GameplayState::Normal: {
            hotbarGui.draw(registry);
            SimulationTime::draw(registry);
            ClickInfoSystem::update(registry);
            drawShipEditorButton(gameState);
            break;
        }
        case GameplayState::Paused: {
            pauseMenu.draw(gameState);
            break;
        }
        case GameplayState::ShipEditor: {
            ShipEditor::draw(registry);
            break;
        }
    }
}

void GuiManager::drawShipEditorButton(GameState &gameState) const {
    ImGuiIO &                  io = ImGui::GetIO();
    const ImVec2               buttonSize(200, 75);
    const ImVec2               windowPosition((io.DisplaySize.x - buttonSize.x) / 2, 0);
    constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                       ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                                       ImGuiWindowFlags_NoSavedSettings |
                                       ImGuiWindowFlags_NoBackground;
    ImGui::SetNextWindowPos(windowPosition, ImGuiCond_Always);
    ImGui::Begin("##ShipEditor", nullptr, flags);
    if (ImGui::Button("Ship Editor", buttonSize)) {
        enterShipEditorSignal.publish(gameState.registry);
        gameState.gameplayState = GameplayState::ShipEditor;
    }
    ImGui::End();
}

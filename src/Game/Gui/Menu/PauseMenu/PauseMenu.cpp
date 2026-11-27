#include "PauseMenu.hpp"

#include <imgui.h>

#include "SettingsMenu.hpp"

void PauseMenu::draw(GameState &gameState) {
    const ImGuiIO &io = ImGui::GetIO();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(io.DisplaySize);

    constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                       ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                                       ImGuiWindowFlags_NoSavedSettings |
                                       ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGui::Begin("PauseMenu", nullptr, flags);

    const ImVec2    windowSize   = ImGui::GetWindowSize();
    constexpr float buttonWidth  = 400.0f;
    constexpr float buttonHeight = 80.0f;
    constexpr float spacing      = 10.0f;
    constexpr int   numButtons   = 3;

    constexpr float totalHeight = buttonHeight * numButtons + spacing * (numButtons - 1);
    const float     startY      = (windowSize.y - totalHeight) / 2.0f;

    ImGui::SetCursorPos(ImVec2((windowSize.x - buttonWidth) / 2.0f, startY));

    ImGui::BeginGroup();

    if (ImGui::Button("Resume", ImVec2(buttonWidth, buttonHeight))) { gameState.menuStack.pop(); }

    ImGui::SetCursorPosX((windowSize.x - buttonWidth) / 2.0f);
    ImGui::Dummy(ImVec2(0, spacing));
    ImGui::SetCursorPosX((windowSize.x - buttonWidth) / 2.0f);

    if (ImGui::Button("Settings", ImVec2(buttonWidth, buttonHeight))) {
        gameState.menuStack.push(std::make_unique<SettingsMenu>());
    }

    ImGui::SetCursorPosX((windowSize.x - buttonWidth) / 2.0f);
    ImGui::Dummy(ImVec2(0, spacing));
    ImGui::SetCursorPosX((windowSize.x - buttonWidth) / 2.0f);

    if (ImGui::Button("Exit to Main Menu", ImVec2(buttonWidth, buttonHeight))) {
        gameState.status = GameStatus::MainMenu;
    }

    ImGui::SetCursorPosX((windowSize.x - buttonWidth) / 2.0f);
    ImGui::Dummy(ImVec2(0, spacing));
    ImGui::SetCursorPosX((windowSize.x - buttonWidth) / 2.0f);

    if (ImGui::Button("Exit", ImVec2(buttonWidth, buttonHeight))) {
        gameState.status = GameStatus::Exiting;
    }

    ImGui::EndGroup();
    ImGui::End();
}

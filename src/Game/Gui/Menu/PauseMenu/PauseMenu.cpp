#include "PauseMenu.hpp"

#include <imgui.h>

auto PauseMenu::draw(GameState &gameState) -> void {
    checkPauseMenuState(gameState);
    if (pauseMenuState != PauseMenuState::None) { return; }

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

    if (ImGui::Button("Resume", ImVec2(buttonWidth, buttonHeight))) {
        pauseMenuState = PauseMenuState::Resume;
    }

    ImGui::SetCursorPosX((windowSize.x - buttonWidth) / 2.0f);
    ImGui::Dummy(ImVec2(0, spacing));
    ImGui::SetCursorPosX((windowSize.x - buttonWidth) / 2.0f);

    if (ImGui::Button("Settings", ImVec2(buttonWidth, buttonHeight))) {
        pauseMenuState = PauseMenuState::Settings;
    }

    ImGui::SetCursorPosX((windowSize.x - buttonWidth) / 2.0f);
    ImGui::Dummy(ImVec2(0, spacing));
    ImGui::SetCursorPosX((windowSize.x - buttonWidth) / 2.0f);

    if (ImGui::Button("Exit to Main Menu", ImVec2(buttonWidth, buttonHeight))) {
        pauseMenuState = PauseMenuState::ExitToMainMenu;
    }

    ImGui::SetCursorPosX((windowSize.x - buttonWidth) / 2.0f);
    ImGui::Dummy(ImVec2(0, spacing));
    ImGui::SetCursorPosX((windowSize.x - buttonWidth) / 2.0f);

    if (ImGui::Button("Exit", ImVec2(buttonWidth, buttonHeight))) {
        pauseMenuState = PauseMenuState::Exit;
    }

    ImGui::EndGroup();
    ImGui::End();
    checkPauseMenuState(gameState);
}

void PauseMenu::checkPauseMenuState(GameState &gameState) {
    switch (pauseMenuState) {
        case PauseMenuState::None: { break; }
        case PauseMenuState::Resume: {
            pauseMenuState          = PauseMenuState::None;
            gameState.gameplayState = GameplayState::Normal;
            break;
        }
        case PauseMenuState::Settings: {
            std::optional<PauseMenuState> newState = settingsMenu.update(gameState);
            if (newState.has_value()) { pauseMenuState = newState.value(); }
            break;
        }
        case PauseMenuState::ExitToMainMenu: {
            gameState.status = GameStatus::MainMenu;
            break;
        }
        case PauseMenuState::Exit: {
            gameState.status = GameStatus::Exiting;
            break;
        }
    }
}
